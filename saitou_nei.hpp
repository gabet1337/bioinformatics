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
  dist_matrix D;
  labels l;
};

saitou_nei::saitou_nei(const char *file) {
  phylib_reader pr;
  pr.read(file, l, D);
}

std::string saitou_nei::compute() {
  parser p;
  int S = l.size();
  std::vector<bool> deleted_row(S,false);
  std::vector<bool> deleted_col(S,false);
  //construct the starting tree T
  node* center = new node();
  // center->name = "center";
  std::vector<node*> leafs(S, 0);
  for (int i = 0; i < S; i++) {
    leafs[i] = new node();
    leafs[i]->name = l[i];
    add_edge(leafs[i], center, 0.0);
  }
  // p.print(center);

  while (S > 3) {
    // std::cout << S << std::endl;
    //compute the matrix N = (n_ij) where n_ij = d_ij - (r_i + r_j)
    // r_i = 1 / (S-2) * sum(d_im)
    std::vector<weight_t> ris(D.size());
    weight_t r_i;
#pragma omp parallel for schedule(dynamic,1) reduction(+ : r_i)
    for (int i = 0; i < D.size(); i++) {
      if (deleted_row[i]) continue;
      r_i = 0.0;
      for (int j = 0; j < D.size(); j++) {
        //compute sum of the row:
        if (deleted_col[j]) continue;
        r_i += D[i][j];
      }
      ris[i] = r_i/(S-2);
      // std::cout << ris[i] << std::endl;
    }
//select i,j such that n_ij is minimum in N
    int best_i = 0, best_j = 0;
    weight_t opti = 1000000000;
// #pragma omp parallel for reduction(min:opti)
    for (int i = 0; i < D.size(); i++) {
      if (deleted_row[i]) continue;
#pragma omp parallel
      {
	weight_t priv_opti = 100000000;
	int priv_best_i = 0;
	int priv_best_j = 0;
#pragma omp for
	for (int j = 0; j < D.size(); j++) {
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
    remove_edge(parent, leafs[best_i]);
    remove_edge(parent, leafs[best_j]);
    remove_edge(leafs[best_i], parent);
    remove_edge(leafs[best_j], parent);
    if (parent->adj_list.empty()) delete parent;
    add_edge(k, parent, 0.0);
    add_edge(k,leafs[best_i], 0.5*(D[best_i][best_j] + ris[best_i] - ris[best_j]));
    add_edge(k,leafs[best_j], 0.5*(D[best_i][best_j] + ris[best_j] - ris[best_i]));
    // p.print(k);

    //update the dissimilarity matrix by deleting rows and columns
    //corresponding to i and j and adding a new row and column for the new taxon k
    //with d_km = 0.5(d_im + d_jm - d_ij)
    deleted_row[best_j] = true;
    deleted_col[best_j] = true;
    leafs[best_i] = k;
#pragma omp parallel for
    for (int k = 0; k < D.size(); k++) {
      if (!deleted_col[k] && !deleted_row[k]) {
        if (k == best_i) D[k][best_i] = 0.0;
        else D[best_i][k] = D[k][best_i] = 0.5*(D[best_i][k]+D[best_j][k]-D[best_i][best_j]);
      }
    }
    S--;
  }

  // let i,j,m be the remaining three taxa.
  // Add a new internal node v to the tree T
  // Add edges (v,i), (v,j) and (v,m) to the Tree T
  std::vector<int> remaining;
  for (int i = 0; i < D.size(); i++) {
    if (!deleted_row[i]) {
      remaining.push_back(i);
    }
  }
  int i = remaining[0], j = remaining[1], m = remaining[2];
  node* v = new node();
  node* parent = leafs[i]->adj_list[0].first;
  remove_edge(parent, leafs[i]);
  remove_edge(parent, leafs[j]);
  remove_edge(parent, leafs[m]);
  remove_edge(leafs[i], parent);
  remove_edge(leafs[j], parent);
  remove_edge(leafs[m], parent);
  if (parent->adj_list.empty()) delete parent;
  add_edge(v, leafs[i], (D[i][j] + D[i][m] - D[j][m]) / 2.0);
  add_edge(v, leafs[j], (D[i][j] + D[j][m] - D[i][m]) / 2.0);
  add_edge(v, leafs[m], (D[i][m] + D[j][m] - D[i][j]) / 2.0);
  return p.parse(v);
}

void saitou_nei::add_edge(node* k, node* n, weight_t w) {
  k->adj_list.push_back({n,w});
  n->adj_list.push_back({k,w});
}

void saitou_nei::remove_edge(node* p, node* n) {
  for (auto x = p->adj_list.begin(); x != p->adj_list.end(); x++)
    if (x->first == n) {
      p->adj_list.erase(x);
      break;
    }
}


#endif
