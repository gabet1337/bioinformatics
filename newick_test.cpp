#include <iostream>
#include <map>
#include "newick_parser.hpp"
using namespace std;
map<node*, int> mni;
int next_id = 0;
map<node*, int> visited;
void print(node* n) {
  visited[n] = 1;
  if (mni.find(n) == mni.end()) mni[n] = next_id++;
  cout << mni[n] << " [label=\"" << n->name << "\"]" << endl;
  for (int i = 0; i < n->adj_list.size(); i++) {
    if (visited[n->adj_list[i].first]) continue;
    if (mni.find(n->adj_list[i].first) == mni.end()) mni[n->adj_list[i].first] = next_id++;
    cout << mni[n] << " -> " << mni[n->adj_list[i].first]
         << " [label=\"" << n->adj_list[i].second << "\"]" << endl;
    print(n->adj_list[i].first);
  }
}

int main() {
  parser p;
  //cout << p.find_match("(ab(asd)asd):0.00123,(asdad)").second << endl;
  node* n = p.parse("tree2.new");

  cout << "digraph g {" << endl;
  print(n);
  cout << "}" << endl;

  return 0;
}
