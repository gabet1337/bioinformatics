#include "newick_parser.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;
typedef pair<int,double> PID;
typedef vector<PID> vid;
typedef vector<vid> vvid;
typedef pair<int,int> ii;
int nextid = 0;
void rename_t1(node* tree, map<node*, int> &visited, map<string, int> &df) {
  visited[tree] = 1;
  if (tree->name != "") df[tree->name] = nextid++;
  for (auto &n : tree->adj_list) {
    if (visited.find(n.first) == visited.end())
      rename_t1(n.first, visited, df);
  }
}

void rename_t2(node* tree, map<node*,int> &visited, map<string, int> &df, map<string,int> &df2) {
  visited[tree] = 1;
  if (df.find(tree->name) != df.end()) df2[tree->name] = df[tree->name];
  for (auto &n : tree->adj_list)
    if (visited.find(n.first) == visited.end())
      rename_t2(n.first, visited, df, df2);
}

void annotate(node* p, node* tree, map<node*,int> &visited, map<string,int> &df, map<node*,ii> &ints, vector<ii> &intervals, map<node*, int> &sizes, int &splits) {
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
        mi = min(mi,ints[n.first].first);
        ma = max(ma,ints[n.first].second);
      }
    }
    splits++;
    // cout << mi << " " << ma << " " << s << endl;
    ints[tree] = ii(mi,ma);
    sizes[tree] = s;
    if (ma-mi+1 == s) intervals.push_back(ii(mi,ma));
  }
}

int main(int argc, char *argv[]) {

  if (argc != 3) {
    //usage
    cout << "Usage: " << argv[0] << " <tree1> <tree2>" << endl;
    return 0;
  }
  parser p;
  cout << "Parsing " << argv[1] << " and " << argv[2] << endl;

  node* tree1 = p.parse(argv[1]);
  node* tree2 = p.parse(argv[2]);

  //root the two trees at the same leaf
  //1. find a leaf in tree1 and make that root.
  queue<node*> q;
  q.push(tree1);
  while (!q.empty()) {
    node* f = q.front(); q.pop();
    if (f->name != "") {tree1 = f; break;}
    for (auto &n : f->adj_list)
      if (n.first != f) q.push(n.first);
  }
  cout << argv[1] << " rooted at: " << tree1->name << endl;
  //2. find same leaf in tree2 and make that root
  q = queue<node*>();
  q.push(tree2);
  while (!q.empty()) {
    node* f = q.front(); q.pop();
    if (f->name == tree1->name) {tree2 = f; break;}
    for (auto &n : f->adj_list)
      if (n.first != f) q.push(n.first);
  }
  cout << argv[2] << " rooted at: " << tree2->name << endl;

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
  map<string,int> df;
  map<node*, int> visited;
  rename_t1(tree1, visited, df);
  //rename the leaves in T2 cf. the df-numbering of leaves in T1
  visited.clear();
  map<string, int> df2;
  rename_t2(tree2, visited, df, df2);

  //annotate internal nodes of T1 with their intervals according to DF-numbering
  vector<ii> intervals_t1, intervals_t2;
  visited.clear();
  //void annotate(node* tree, map<node*,int> &visited, map<string,int> &df, map<node*,ii> &ints, vector<ii> &intervals, map<node*, int> &sizes) {
  int splits_t1 = 0, splits_t2 = 0;
  map<node*,ii> ints;
  map<node*,int> sizes;
  cout << "annotating " << argv[1] << endl;
  annotate(0,tree1, visited, df, ints, intervals_t1, sizes, splits_t1);
  visited.clear(); ints.clear(); sizes.clear();
  cout << "annotating " << argv[2] << endl;
  annotate(0,tree2, visited, df2, ints, intervals_t2, sizes, splits_t2);

  sort(intervals_t1.begin(), intervals_t1.end());
  sort(intervals_t2.begin(), intervals_t2.end());
  vector<ii> intersection;

  // cout << splits_t1 << " " << splits_t2 << endl;

  // cout << intervals_t1.size() << endl;
  // cout << intervals_t2.size() << endl;

  set_intersection(intervals_t1.begin(), intervals_t1.end(), intervals_t2.begin(), intervals_t2.end(), back_inserter(intersection));

  // cout << intersection.size() << endl;

  cout << "Robinson-Foulds distance is: " << splits_t1+splits_t2 - 2*intersection.size() << endl;

  return 0;
}
