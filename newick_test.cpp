#include <iostream>
#include <map>
#include <queue>
#include "newick_parser.hpp"
using namespace std;
// map<node*, int> mni;
// int next_id = 0;
// map<node*, int> visited;
// void print(node* n) {
//   visited[n] = 1;
//   if (mni.find(n) == mni.end()) mni[n] = next_id++;
//   cout << mni[n] << " [label=\"" << n->name << "\"]" << endl;
//   for (int i = 0; i < n->adj_list.size(); i++) {
//     if (visited[n->adj_list[i].first]) continue;
//     if (mni.find(n->adj_list[i].first) == mni.end()) mni[n->adj_list[i].first] = next_id++;
//     cout << mni[n] << " -> " << mni[n->adj_list[i].first]
//          << " [label=\"" << n->adj_list[i].second << "\"]" << endl;
//     print(n->adj_list[i].first);
//   }
// }

int main() {
  parser p;
  //cout << p.find_match("(ab(asd)asd):0.00123,(asdad)").second << endl;
  node* tree1 = p.parse("tree1.new");
  //find seq3
  queue<node*> q; q.push(tree1);
  while (!q.empty()) {
    node* f = q.front(); q.pop();
    if (f->name == "seq3") { tree1 = f; break;}
    for (auto &k : f->adj_list)
      if (k.first != f) q.push(k.first);
  }
  for (int i = 0; i < tree1->adj_list[0].first->adj_list.size(); i++)
    if (tree1 == tree1->adj_list[0].first->adj_list[i].first) {
      tree1->adj_list[0].first->adj_list.erase(tree1->adj_list[0].first->adj_list.begin()+i); break;
    }
  tree1 = tree1->adj_list[0].first;


  cout << "digraph g {" << endl;
  p.print(tree1);
  cout << "}" << endl;

  return 0;
}
