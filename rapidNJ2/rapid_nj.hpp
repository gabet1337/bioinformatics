#ifndef RAPIDNJ_HPP
#define RAPIDNJ_HPP
#include "phylip_parser.hpp"
#include "newick_parser.hpp"
#include <map>
#include <algorithm>

class rapid_nj {

public:
  rapid_nj();
  node* construct_tree(std::string file);
  node* construct_tree_rapid(std::string file);
  void print(node* n);
private:
  std::vector<std::vector<std::pair<double,int> > > dist;
  std::vector<std::vector<std::pair<double,int> > > dist_sort;
  std::vector<std::string> labels;
  void print_r(node* n);
  std::map<node*, int> visited;
  std::map<node*, int> mni;
  int next_id = 0;
  node* initialization();
  std::vector<node*> S;
  std::vector<bool> deleted;
  std::pair<std::pair<int,double>,std::pair<int,double> > get_min_tax_pair();
  std::pair<std::pair<int,double>,std::pair<int,double> > get_min_tax_pair_rapid();
  void update_dist(int i, int j);
  double r(int i);
  void remove_edge(node* p, node* n);
  void add_edge(node* p, node* n, double weight);
  size_t size;
  void clone_sort_d();
  std::vector<double> row_sums;
  void compute_row_sums();
  double u_max_idx = 0;
  void update_dist_rapid(int i, int j);
};

rapid_nj::rapid_nj() {};

node* rapid_nj::initialization() {

  node* root = new node();
 
  for (int i = 0; i < dist.size(); i++) {
    node* leaf = new node();
    leaf->name = labels[i];
    add_edge(leaf,root,0.0);
    S.push_back(leaf);
  }

  deleted.assign(S.size(), false);

  size = S.size();
  
  return root;
  
}

void rapid_nj::compute_row_sums() {

  row_sums.clear();
  
  double u_max = 0;
  
  for (int i = 0; i < dist.size(); i++) {
    double d = r(i);
    if (d > u_max) {
      u_max = d;
      u_max_idx = i;
    }
    row_sums.push_back(d);
  }
  
}

void rapid_nj::clone_sort_d() {

  dist_sort.clear();
  dist_sort.resize(dist.size());
  
  for (int i = 0; i < dist.size(); i++) {
    for (int j = 0; j < dist.size(); j++) {
      dist_sort[i].push_back(std::make_pair(dist[i][j].first, j));
    }
    std::sort(dist_sort[i].begin(), dist_sort[i].end());
    for (int k = 0; k < dist.size(); k++) {
      dist[i][dist_sort[i][k].first].second = k;
    }
  }
  
}

node* rapid_nj::construct_tree_rapid(std::string file) {

  phylip_parser pp;
  pp.parse(file, dist, labels);

  node* root = initialization();

  //compute_row_sums();
  clone_sort_d();
  
  while (size > 3) {

    std::cout << size << std::endl;
    
    compute_row_sums();
    
    // Select minimizing (i,j)
    std::pair<std::pair<int,double>,std::pair<int,double> > cand = get_min_tax_pair_rapid();
    int i = cand.first.first;
    int j = cand.second.first;
    double ri = cand.first.second;
    double rj = cand.second.second;

    node* node_i = S[i];
    node* node_j = S[j];
    node* parent = S[i]->adj_list[0].first;

    // Remove old (i,j) edges from tree.
    remove_edge(node_i, parent);
    remove_edge(node_j, parent);
    remove_edge(parent, node_i);
    remove_edge(parent, node_j);

    // Add a new node k to the tree T
    node* k = new node();   
    parent->adj_list.push_back(std::make_pair(k, 0));
    k->adj_list.push_back(std::make_pair(parent,0));
    
    // Add edges (k,i) and (k,j) with weights 1/2(dij + ri - rj)
    add_edge(node_i, k, 0.5 * (dist[i][j].first + ri - rj));
    add_edge(node_j, k, 0.5 * (dist[i][j].first + rj - ri));

    // Update the dissimilarity matrix.
    update_dist_rapid(i,j);

    // Delete i,j from S.
    deleted[i] = true;
    deleted[j] = true;
    size -= 2;

    //compute_row_sums();
    
    // Add taxon k to S.
    S.push_back(k);
    deleted.push_back(false);
    size += 1;

    //print(root);

  }

  // Termination. Let i,j.m be the remaining taxa.
  int term_i = -1, term_j = -1, term_m = -1;

  for (int i = 0; i < dist.size(); i++) {
    if (!deleted[i]) {
      if (term_i == -1)
	term_i = i;
      else if (term_j == -1)
	term_j = i;
      else
	term_m = i;
    }
  }

  node* term_i_node = S[term_i];
  node* term_j_node = S[term_j];
  node* term_m_node = S[term_m];
  node* term_parent = term_i_node->adj_list[0].first;
  
  remove_edge(term_parent, term_i_node);
  remove_edge(term_parent, term_j_node);
  remove_edge(term_parent, term_m_node);
  remove_edge(term_m_node, term_parent);
  remove_edge(term_j_node, term_parent);
  remove_edge(term_i_node, term_parent);

  
  // Add new internal node v to the Tree.
  node* v = new node();

  if (term_parent->adj_list.size() == 0) {
    root = v;
  } else {
    add_edge(v, term_parent, 0.0);
  }
  
  // Connect (v,i)
  add_edge(v, term_i_node, 0.5 * (dist[term_i][term_j].first + dist[term_i][term_m].first - dist[term_j][term_m].first));

  // Connect (v,j)
  add_edge(v, term_j_node, 0.5 * (dist[term_i][term_j].first + dist[term_j][term_m].first - dist[term_i][term_m].first));

  // Connect (v,m)
  add_edge(v, term_m_node, 0.5 * (dist[term_i][term_m].first + dist[term_j][term_m].first - dist[term_i][term_j].first));
  
  return root;
  
}

node* rapid_nj::construct_tree(std::string file) {

  phylip_parser pp;
  pp.parse(file, dist, labels);

  node* root = initialization();

  //compute_row_sums();
  //clone_sort_d();
  
  while (size > 3) {

    // Select minimizing (i,j)
    std::pair<std::pair<int,double>,std::pair<int,double> > cand = get_min_tax_pair();
    int i = cand.first.first;
    int j = cand.second.first;
    double ri = cand.first.second;
    double rj = cand.second.second;

    node* node_i = S[i];
    node* node_j = S[j];
    node* parent = S[i]->adj_list[0].first;

    // Remove old (i,j) edges from tree.
    remove_edge(node_i, parent);
    remove_edge(node_j, parent);
    remove_edge(parent, node_i);
    remove_edge(parent, node_j);

    // Add a new node k to the tree T
    node* k = new node();   
    parent->adj_list.push_back(std::make_pair(k, 0));
    k->adj_list.push_back(std::make_pair(parent,0));
    
    // Add edges (k,i) and (k,j) with weights 1/2(dij + ri - rj)
    add_edge(node_i, k, 0.5 * (dist[i][j].first + ri - rj));
    add_edge(node_j, k, 0.5 * (dist[i][j].first + rj - ri));

    // Update the dissimilarity matrix.
    update_dist(i,j);

    // Delete i,j from S.
    deleted[i] = true;
    deleted[j] = true;
    size -= 2;

    //compute_row_sums();
    
    // Add taxon k to S.
    S.push_back(k);
    deleted.push_back(false);
    size += 1;

    //print(root);

  }

  // Termination. Let i,j.m be the remaining taxa.
  int term_i = -1, term_j = -1, term_m = -1;

  for (int i = 0; i < dist.size(); i++) {
    if (!deleted[i]) {
      if (term_i == -1)
	term_i = i;
      else if (term_j == -1)
	term_j = i;
      else
	term_m = i;
    }
  }

  node* term_i_node = S[term_i];
  node* term_j_node = S[term_j];
  node* term_m_node = S[term_m];
  node* term_parent = term_i_node->adj_list[0].first;
  
  remove_edge(term_parent, term_i_node);
  remove_edge(term_parent, term_j_node);
  remove_edge(term_parent, term_m_node);
  remove_edge(term_m_node, term_parent);
  remove_edge(term_j_node, term_parent);
  remove_edge(term_i_node, term_parent);

  
  // Add new internal node v to the Tree.
  node* v = new node();

  if (term_parent->adj_list.size() == 0) {
    root = v;
  } else {
    add_edge(v, term_parent, 0.0);
  }
  
  // Connect (v,i)
  add_edge(v, term_i_node, 0.5 * (dist[term_i][term_j].first + dist[term_i][term_m].first - dist[term_j][term_m].first));

  // Connect (v,j)
  add_edge(v, term_j_node, 0.5 * (dist[term_i][term_j].first + dist[term_j][term_m].first - dist[term_i][term_m].first));

  // Connect (v,m)
  add_edge(v, term_m_node, 0.5 * (dist[term_i][term_m].first + dist[term_j][term_m].first - dist[term_i][term_j].first));
  
  return root;
  
}

void rapid_nj::update_dist(int del_i, int del_j) {

  std::vector<std::pair<double,int> > k_row;

  for (int m = 0; m < dist.size(); m++) {
    if (deleted[m] || m == del_i || m == del_j) {
      k_row.push_back(std::make_pair(0.0,m));
      dist[m].push_back(std::make_pair(0.0,m));
    } else {
      double d = 0.5 * (dist[del_i][m].first + dist[del_j][m].first - dist[del_i][del_j].first);
      k_row.push_back(std::make_pair(d,m));
      dist[m].push_back(std::make_pair(d,m));
    }
  }

  dist.push_back(k_row);

}

void rapid_nj::update_dist_rapid(int del_i, int del_j) {

  std::vector<std::pair<double,int> > k_row;

  for (int m = 0; m < dist.size(); m++) {
    //row_sums[m] -= dist[m][del_i].first / size;
    //row_sums[m] -= dist[m][del_j].first / size;
    if (deleted[m] || m == del_i || m == del_j) {
      k_row.push_back(std::make_pair(0.0,m));
      dist[m].push_back(std::make_pair(0.0,dist.size()));
    } else {
      double d = 0.5 * (dist[del_i][m].first + dist[del_j][m].first - dist[del_i][del_j].first);
      k_row.push_back(std::make_pair(d,m));
      dist[m].push_back(std::make_pair(d,dist.size()));
      //row_sums[m] += d;
    }
    //row_sums[m] = r(m);
  }

  dist.push_back(k_row);

  clone_sort_d();

}

std::pair<std::pair<int,double>,std::pair<int,double> > rapid_nj::get_min_tax_pair() {
  
  int min_i, min_j;
  double min_val = 999999999;
  double min_ri, min_rj;
  for (int i = 0; i < dist.size(); i++) {
    if (deleted[i]) continue;
    for (int j = 0; j < dist.size(); j++) {
      if (i == j) continue;
      if (deleted[j]) continue;
      double cand_ri = r(i);
      double cand_rj = r(j);
      double cand = dist[i][j].first - (cand_ri + cand_rj);
      if (cand < min_val) {
	min_i = i;
	min_j = j;
	min_ri = cand_ri;
	min_rj = cand_rj;
	min_val = cand;
      }
    }
  }

  return std::make_pair(std::make_pair(min_i, min_ri),std::make_pair(min_j, min_rj));
  
}

std::pair<std::pair<int,double>,std::pair<int,double> > rapid_nj::get_min_tax_pair_rapid() {

  int min_i, min_j;
  double min_val = 999999999;
  double min_ri, min_rj;
  for (int i = 0; i < dist_sort.size(); i++) {
    if (deleted[i]) continue;
    for (int j = 0; j < dist_sort.size(); j++) {
      //std::cout << "Index in dist: (" << i << "," <<  dist_sort[i][j].second << ")" << std::endl;
      if (dist_sort[i][j].first - row_sums[i] - row_sums[u_max_idx] >= min_val) break;
      if (i == dist_sort[i][j].second) continue;
      if (deleted[dist_sort[i][j].second]) continue;
      double cand_ri = row_sums[i]; // r(i);
      double cand_rj = row_sums[dist_sort[i][j].second]; // r(j);
      double cand = dist_sort[i][j].first - (cand_ri + cand_rj);
      if (cand < min_val) {
	min_i = i;
	min_j = dist_sort[i][j].second;
	min_ri = cand_ri;
	min_rj = cand_rj;
	min_val = cand;
      }
    }
  }

  return std::make_pair(std::make_pair(min_i, min_ri),std::make_pair(min_j, min_rj));
  
}

double rapid_nj::r(int i) {

  double res = 0;
  
  for (int m = 0; m < dist.size(); m++) {
    if (!deleted[m])
      res += dist[i][m].first;
  }

  res = res / (size-2);

  return res;
  
}

void rapid_nj::print(node* n) {
  visited.clear();
  mni.clear();
  next_id = 0;
  std::cout << "digraph g {" << std::endl;
  print_r(n);
  std::cout << "}" << std::endl;
}


void rapid_nj::print_r(node* n) {
  visited[n] = 1;
  if (mni.find(n) == mni.end()) mni[n] = next_id++;
  std::cout << mni[n] << " [label=\"" << n->name << "\"]" << std::endl;
  for (int i = 0; i < n->adj_list.size(); i++) {
    if (visited[n->adj_list[i].first]) {
      std::cout << mni[n] << " -> " << mni[n->adj_list[i].first] << " [label=\"" << n->adj_list[i].second << "\"]" << std::endl;
      continue;
    }
    if (mni.find(n->adj_list[i].first) == mni.end()) mni[n->adj_list[i].first] = next_id++;
    std::cout << mni[n] << " -> " << mni[n->adj_list[i].first]
              << " [label=\"" << n->adj_list[i].second << "\"]" << std::endl;
    print_r(n->adj_list[i].first);
  }
}

void rapid_nj::add_edge(node* p, node* n, double weight) {
  p->adj_list.push_back(std::make_pair(n, weight));
  n->adj_list.push_back(std::make_pair(p, weight));
}

void rapid_nj::remove_edge(node* p, node* n) {

  for (auto x = p->adj_list.begin(); x != p->adj_list.end(); x++)
    if (x->first == n) {
      p->adj_list.erase(x);
      break;
    }
}

#endif
