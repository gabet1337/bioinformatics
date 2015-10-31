#include <iostream>
#include <map>
#include "newick_parser.hpp"
using namespace std;
map<node*, int> mni;
int next_id = 0;
void print(node* n) {
  if (mni.find(n) == mni.end()) mni[n] = next_id++;
  cout << mni[n] << " [label=\"" << n->name << "\"]" << endl;
  for (int i = 0; i < n->adj_list.size(); i++) {
    if (mni.find(n->adj_list[i]) == mni.end()) mni[n->adj_list[i]] = next_id++;
    cout << mni[n] << " -> " << mni[n->adj_list[i]]
         << " [label=\"" << n->adj_list[i]->weight << "\"]" << endl;
    print(n->adj_list[i]);
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
