#ifndef RFDIST_HPP
#define RFDIST_HPP
#include "newick_parser.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>

typedef std::pair<int,double> PID;
typedef std::vector<PID> vid;
typedef std::vector<vid> vvid;
typedef std::pair<int,int> ii;

class rfdist {
public:
  rfdist();
  int distance(char* t1, char* t2);
private:
  void rename_t1(node* tree, std::map<node*, int> &visited, std::map<std::string, int> &df);
  void rename_t2(node* tree, std::map<node*,int> &visited, std::map<std::string, int> &df, std::map<std::string,int> &df2);
  void annotate(node* p, node* tree, std::map<node*,int> &visited, std::map<std::string,int> &df, std::map<node*,ii> &ints, std::vector<ii> &intervals, std::map<node*, int> &sizes, int &splits);
  int nextid = 0;
};

rfdist::rfdist() {};

int nextid = 0;
void rfdist::rename_t1(node* tree, std::map<node*, int> &visited, std::map<std::string, int> &df) {
  visited[tree] = 1;
  if (tree->name != "") df[tree->name] = nextid++;
  for (auto &n : tree->adj_list) {
    if (visited.find(n.first) == visited.end())
      rename_t1(n.first, visited, df);
  }
}

void rfdist::rename_t2(node* tree, std::map<node*,int> &visited, std::map<std::string, int> &df, std::map<std::string,int> &df2) {
  visited[tree] = 1;
  if (df.find(tree->name) != df.end()) df2[tree->name] = df[tree->name];
  for (auto &n : tree->adj_list)
    if (visited.find(n.first) == visited.end())
      rename_t2(n.first, visited, df, df2);
}

void rfdist::annotate(node* p, node* tree, std::map<node*,int> &visited, std::map<std::string,int> &df, std::map<node*,ii> &ints, std::vector<ii> &intervals, std::map<node*, int> &sizes, int &splits) {
  visited[tree] = 1;
  for (auto &n : tree->adj_list)
    if (visited.find(n.first) == visited.end())
      annotate(tree, n.first, visited, df, ints, intervals, sizes,splits);
  if (p == 0) return;
  if (df.find(tree->name) != df.end()) {
    ints[tree] = ii(df[tree->name], df[tree->name]);
    sizes[tree] = 1;
    // cout << tree->name << " " << df[tree->name] << endl;
  } else {
    //internal node! gather min and max from all children!
    int mi = 1000000, ma = -1000000;
    int s = 0;
    for (auto &n : tree->adj_list) {
      // cout << tree << " " << n.first << endl;
      if (n.first != p) {
        s += sizes[n.first];
        mi = std::min(mi,ints[n.first].first);
        ma = std::max(ma,ints[n.first].second);
      }
    }
    splits++;
    // cout << mi << " " << ma << " " << s << endl;
    ints[tree] = ii(mi,ma);
    sizes[tree] = s;
    if (ma-mi+1 == s) intervals.push_back(ii(mi,ma));
  }
}

int rfdist::distance(char* t1, char* t2) {
  nextid = 0;
  parser p;
  node* tree1 = p.parse(t1);
  node* tree2 = p.parse(t2);

  //root the two trees at the same leaf
  //1. find a leaf in tree1 and make that root.
  std::queue<std::pair<node*,node*> > q;
  q.push(std::make_pair(tree1,(node*)0));
  while (!q.empty()) {
    std::pair<node*,node*> f = q.front(); q.pop();
    if (f.first->name != "") {tree1 = f.first; break;}
    for (auto &n : f.first->adj_list)
      if (n.first != f.second) q.push(std::make_pair(n.first,f.first));
  }

  std::cout << "rooted " << t1 << " in " << tree1->name << std::endl;
  //2. find same leaf in tree2 and make that root
  q = std::queue<std::pair<node*,node*> >();
  q.push(std::make_pair(tree2,(node*)0));
  while (!q.empty()) {
    std::pair<node*,node*> f = q.front(); q.pop();
    if (f.first->name == tree1->name) {tree2 = f.first; break;}
    for (auto &n : f.first->adj_list)
      if (n.first != f.second) q.push(std::make_pair(n.first, f.first));
  }

  std::cout << "rooted " << t2 << " in " << tree2->name << std::endl;

  //remove the root nodes from the trees and make is only child the root:
  for (int i = 0; i < tree1->adj_list[0].first->adj_list.size(); i++)
    if (tree1 == tree1->adj_list[0].first->adj_list[i].first) {
      tree1->adj_list[0].first->adj_list.erase(tree1->adj_list[0].first->adj_list.begin()+i); break;
    }
  tree1 = tree1->adj_list[0].first;
  for (int i = 0; i < tree2->adj_list[0].first->adj_list.size(); i++)
    if (tree2 == tree2->adj_list[0].first->adj_list[i].first) {
      tree2->adj_list[0].first->adj_list.erase(tree2->adj_list[0].first->adj_list.begin()+i); break;
    }
  tree2 = tree2->adj_list[0].first;

  //make a depth first numbering of the leaves in T1
  std::map<std::string,int> df;
  std::map<node*, int> visited;
  std::cout << "renaming " << t1 << std::endl;
  rename_t1(tree1, visited, df);
  //rename the leaves in T2 cf. the df-numbering of leaves in T1
  visited.clear();
  std::map<std::string, int> df2;
  std::cout << "renaming " << t2 << std::endl;
  rename_t2(tree2, visited, df, df2);

  //annotate internal nodes of T1 with their intervals according to DF-numbering
  std::vector<ii> intervals_t1, intervals_t2;
  visited.clear();
  //void annotate(node* tree, map<node*,int> &visited, map<string,int> &df, map<node*,ii> &ints, vector<ii> &intervals, map<node*, int> &sizes) {
  int splits_t1 = 0, splits_t2 = 0;
  std::map<node*,ii> ints;
  std::map<node*,int> sizes;
  std::cout << "annotating " << t1 << std::endl;
  annotate(0,tree1, visited, df, ints, intervals_t1, sizes, splits_t1);
  visited.clear(); ints.clear(); sizes.clear();
  std::cout << "annotating " << t2 << std::endl;
  annotate(0,tree2, visited, df2, ints, intervals_t2, sizes, splits_t2);
  sort(intervals_t1.begin(), intervals_t1.end());
  sort(intervals_t2.begin(), intervals_t2.end());
  std::vector<ii> intersection;

  std::set_intersection(intervals_t1.begin(), intervals_t1.end(), intervals_t2.begin(), intervals_t2.end(), std::back_inserter(intersection));

  return splits_t1+splits_t2 - 2*intersection.size();

  return 0;
}
#endif
