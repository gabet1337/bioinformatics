#include <iostream>
#include "newick_parser.hpp"
#include "rfdist.hpp"
#include "test_lib.hpp"
#include <fstream>
#include <queue>
using namespace std;
typedef pair<node*, int> ni;

void write(string t, string file) {
  ofstream f(file, ios::out);
  f << t;
  f.close();
}

int main() {
  
  for (int i = 2; i < 22; i++) {
    int nodes = 0;
    queue<ni> q;
    node* root = new node();
    q.push(ni(root,1));
    while (true) {
      ni f = q.front(); q.pop();
      if (f.second == i) break;
      nodes+=2;
      node* n = new node();
      node* n1 = new node();
      f.first->adj_list.push_back(make_pair(n,0.0));
      n->adj_list.push_back(make_pair(f.first,0.0));

      f.first->adj_list.push_back(make_pair(n1,0.0));
      n1->adj_list.push_back(make_pair(f.first,0.0));

      if (f.second == i-1) {
        n->name = "id"+std::to_string(nodes-1);
        n1->name = "id"+std::to_string(nodes);
      }
      q.push(ni(n,f.second+1));
      q.push(ni(n1,f.second+1));
    }

    
    string filename = (to_string(i)+".new");
    char* tr = &filename[0u];
    parser p;
    string tree = p.parse(root);
    write(tree,filename);
    test::clock c;
    rfdist rf;
    c.start();
    rf.distance(tr,tr);
    c.stop();
    cout << nodes << "\t" << c.count() << endl;
    delete root;
  }

  return 0;
}
