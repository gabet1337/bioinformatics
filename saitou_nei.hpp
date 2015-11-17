#ifndef SAITOU_NEI_HPP
#define SAITOU_NEI_HPP

#include "phylib_reader.hpp"
#include "newick_parser.hpp"
#include <string>
#include <vector>

class saitou_nei {

public:
  saitou_nei(const char *file);
  std::string compute();
private:
  dist_matrix D;
  labels l;
};

saitou_nei::saitou_nei(const char *file) {
  phylib_reader pr;
  pr.read(file, l, D);
}

std::string saitou_nei::compute() {
  parser p;
  int N = l.size();
  //construct the starting tree T
  node* center = new node();
  std::vector<node*> leafs(N, 0);
  for (int i = 0; i < N; i++) {
    leafs[i] = new node();
    leafs[i]->name = l[i];
    leafs[i]->adj_list.push_back({center, 0.0});
    center->adj_list.push_back({leafs[i], 0.0});
  }
  p.print(center);

  int S = N;
  while (S > 3) {
    
  }

  return "";
}


#endif
