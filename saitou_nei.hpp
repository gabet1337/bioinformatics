#ifndef SAITOU_NEI_HPP
#define SAITOU_NEI_HPP

#include "phylib_reader.hpp"
#include "newick_parser.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <omp.h>

class saitou_nei {

public:
  saitou_nei(const char *file);
  std::string compute();
private:
  void add_edge(node* k, node* n, weight_t w);
  void remove_edge(node* p, node *n);
  void remove_edges(node* p, node** ns, int num);
  void garbage_collect(const int S, std::vector<bool> &deleted_row,
                       std::vector<bool> &deleted_col, std::vector<node*> &leafs);
  dist_matrix D;
  labels l;
};

saitou_nei::saitou_nei(const char *file) {
  phylib_reader pr;
  pr.read_self(file, l, D);
}

void saitou_nei::garbage_collect(const int S, std::vector<bool> &deleted_row,
                                 std::vector<bool> &deleted_col, std::vector<node*> &leafs) {
  // std::cout << "NOW GARBAGE COLLECTING" << std::endl;
  dist_matrix new_D(S, std::vector<weight_t>(S));
  std::vector<node*> new_leafs(S);

//   unsigned int prefix_rows[deleted_row.size()], prefix_cols[deleted_col.size()];
//   prefix_rows[0] = deleted_row[0];
//   prefix_cols[0] = deleted_col[0];
//   unsigned int size = D.size();  
//   for (unsigned int i = 1; i < size; ++i) {
//     prefix_rows[i] = prefix_rows[i-1]+deleted_row[i];
//     prefix_cols[i] = prefix_rows[i-1]+deleted_col[i];
//   }

// #pragma parallel for
//   for (unsigned int i = 0; i < size; ++i) {
//     if (!deleted_row[i]) {
//       unsigned row = i-prefix_rows[i];
// #pragma parallel for
//       for (unsigned int j = 0; j < size; ++j) {
// 	if (!deleted_col[j]) {
// 	  new_D[row][j-prefix_cols[j]] = D[i][j];
// 	}
//       }
//       new_leafs[row] = leafs[i];
//     }
//   }

  unsigned int row = 0, col = 0, size = D.size();
  for (unsigned int i = 0; i < size; ++i) {
    if (deleted_row[i]) continue;
    col = 0;
    for (unsigned int j = 0; j < size; ++j) {
      if (!deleted_col[j]) {
  	new_D[row][col] = D[i][j];
  	++col;
      }
    }
    new_leafs[row] = leafs[i];
    ++row;
  }

  deleted_row = std::vector<bool>(S,false);
  deleted_col = std::vector<bool>(S,false);
  std::swap(new_D, D);
  std::swap(leafs, new_leafs);
}

std::string saitou_nei::compute() {
  unsigned int S = l.size();
  unsigned int threshold = 0.9*(double)S;
  unsigned int chunk_size = S/32;
  unsigned int Dsize = D.size();
  
  std::vector<bool> deleted_row(S,false);
  std::vector<bool> deleted_col(S,false);
  //construct the starting tree T
  node* center = new node();
  // center->name = "center";
  std::vector<node*> leafs(S);

// #pragma omp parallel for schedule(dynamic,chunk_size)
  for (unsigned int i = 0; i < S; i++) {
    leafs[i] = new node();
    leafs[i]->name = l[i];
    add_edge(leafs[i], center, 0.0);
  }
  // p.print(center);

  while (S > 3) {
    // std::cout << S << std::endl;
    //compute the matrix N = (n_ij) where n_ij = d_ij - (r_i + r_j)
    // r_i = 1 / (S-2) * sum(d_im)

    std::vector<weight_t> ris(Dsize);
    weight_t r_i;
#pragma omp parallel for schedule(dynamic,chunk_size) reduction(+ : r_i)
    for (unsigned int i = 0; i < Dsize; ++i) {
      if (deleted_row[i]) continue;
      r_i = 0.0;
      for (unsigned int j = 0; j < Dsize; ++j) {
        //compute sum of the row:
        if (!deleted_col[j]) r_i += D[i][j];
      }
      ris[i] = r_i/(S-2);
    }
    //select i,j such that n_ij is minimum in N
    unsigned int best_i = 0, best_j = 0;
    weight_t opti = 1000000000;

    for (unsigned int i = 0; i < Dsize; ++i) {
      if (deleted_row[i]) continue;
#pragma omp parallel
      {
	weight_t priv_opti = 100000000;
	unsigned int priv_best_i = 0;
	unsigned int priv_best_j = 0;
#pragma omp for
	for (unsigned int j = 0; j < Dsize; ++j) {
	  if (i == j || deleted_col[j]) continue;
	  if (D[i][j] - (ris[i] + ris[j]) < priv_opti) {
	    priv_opti = D[i][j] - (ris[i] + ris[j]);
	    priv_best_i = i;
	    priv_best_j = j;
	  }
	}
#pragma omp flush (opti)
	if (priv_opti < opti) {
	  #pragma omp critical
	  {
	    if (priv_opti < opti) {
	      opti = priv_opti;
	      best_i = priv_best_i;
	      best_j = priv_best_j;
	    }
	  }
	}
      }
    }

    //add a new node k to the tree T:
    node* k = new node();
    //add edges (k,i) and (k,j) with appropriate weights
    node* parent = leafs[best_i]->adj_list[0].first;
    node* li = leafs[best_i];
    node* lj = leafs[best_j];
    node* edges_to_remove[2] = {li, lj};
    remove_edges(parent, edges_to_remove, 2);

    remove_edge(li, parent);
    remove_edge(lj, parent);

    add_edge(k, parent, 0.0);
    add_edge(k,li, (D[best_i][best_j] + ris[best_i] - ris[best_j])*0.5);
    add_edge(k,lj, (D[best_i][best_j] + ris[best_j] - ris[best_i])*0.5);


    //update the dissimilarity matrix by deleting rows and columns
    //corresponding to i and j and adding a new row and column for the new taxon k
    //with d_km = 0.5(d_im + d_jm - d_ij)
    deleted_row[best_j] = true;
    deleted_col[best_j] = true;
    leafs[best_i] = k;
#pragma omp parallel for
    for (unsigned int k = 0; k < Dsize; ++k) {
      if (!deleted_col[k] && !deleted_row[k]) {
        if (k == best_i) D[k][best_i] = 0.0;
        else D[best_i][k] = D[k][best_i] = 0.5*(D[best_i][k]+D[best_j][k]-D[best_i][best_j]);
      }
    }
    --S;
    if (S > 50 && S < threshold) {
      garbage_collect(S, deleted_row, deleted_col, leafs);
      threshold = 0.9*(double)S;
      chunk_size = S/8;
      Dsize = D.size();
    }
  }

  // let i,j,m be the remaining three taxa.
  // Add a new internal node v to the tree T
  // Add edges (v,i), (v,j) and (v,m) to the Tree T
  std::vector<int> remaining;
  for (int i = 0; i < Dsize; ++i) {
    if (!deleted_row[i]) {
      remaining.push_back(i);
    }
  }
  int i = remaining[0], j = remaining[1], m = remaining[2];
  node* v = new node();
  node* parent = leafs[i]->adj_list[0].first;

  node* edges_to_remove[3] = {leafs[i], leafs[j], leafs[m]};
  remove_edges(parent, edges_to_remove, 3);
  remove_edge(leafs[i], parent);
  remove_edge(leafs[j], parent);
  remove_edge(leafs[m], parent);
  if (parent->adj_list.empty()) delete parent;
  add_edge(v, leafs[i], (D[i][j] + D[i][m] - D[j][m]) * 0.5);
  add_edge(v, leafs[j], (D[i][j] + D[j][m] - D[i][m]) * 0.5);
  add_edge(v, leafs[m], (D[i][m] + D[j][m] - D[i][j]) * 0.5);
  parser p;
  return p.parse(v);
}

void saitou_nei::add_edge(node* k, node* n, weight_t w) {
  k->adj_list.push_back({n,w});
  n->adj_list.push_back({k,w});
}

void saitou_nei::remove_edge(node* p, node* n) {
  for (auto x = p->adj_list.begin(); x != p->adj_list.end(); ++x)
    if (x->first == n) {
      p->adj_list.erase(x);
      break;
    }
}

void saitou_nei::remove_edges(node* p, node** ns, int num) {
  int count = 0;
  for (auto x = p->adj_list.end(); x != p->adj_list.begin(); --x) {
    for (int i = 0; i < num; ++i)
      if (x->first == ns[i]) {
	p->adj_list.erase(x);
	if (++count == num) break;
      }
  }
}


#endif
