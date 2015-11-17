#include <iostream>
#include "saitou_nei.hpp"
using namespace std;

int main() {

  saitou_nei sn("phylib/1849_FG-GAP.phy");
  cout << sn.compute() << endl;

  return 0;

}
