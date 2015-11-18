#include "saitou_nei.hpp"
#include "stdio.h"

using namespace std;


int main(int argc, char *argv[]) {
  saitou_nei sn(argv[1]);
  printf("%s\n", sn.compute().c_str());
  return 0;
}

