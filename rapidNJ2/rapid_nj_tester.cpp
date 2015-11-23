#include <iostream>
#include "rapid_nj.hpp"

using namespace std;

int main() {

  rapid_nj nj;
  node* root = nj.construct_tree_rapid("phylip/1849_FG-GAP.phy");  
  nj.print(root);

  return 0;

}

