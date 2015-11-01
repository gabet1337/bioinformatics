#include "newick_parser.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;
typedef pair<int,double> PID;
typedef vector<PID> vid;
typedef vector<vid> vvid;
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
  cout << "tree1 rooted at: " << tree1->name << endl;
  //2. find same leaf in tree2 and make that root
  q = queue<node*>();
  q.push(tree2);
  while (!q.empty()) {
    node* f = q.front(); q.pop();
    if (f->name == tree1->name) {tree2 = f; break;}
    for (auto &n : f->adj_list)
      if (n.first != f) q.push(n.first);
  }
  cout << "tree2 rooted at: " << tree2->name << endl;

  //make a depth first numbering of the leaves in T1

  //rename the leaves in T2 cf. the df-numbering of leaves in T1

  

  return 0;
}
