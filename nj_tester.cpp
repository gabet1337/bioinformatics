#include <iostream>
#include "saitou_nei.hpp"
using namespace std;

int main() {

  saitou_nei sn("phylib/example_slide4.phy");
  cout << sn.compute() << endl;

  return 0;

}
