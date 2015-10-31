#include "newick_parser.hpp"
#include <iostream>
using namespace std;
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

  //make a depth first numbering of the leaves in T1

  //rename the leaves in T2 cf. the df-numbering of leaves in T1

  

  return 0;
}
