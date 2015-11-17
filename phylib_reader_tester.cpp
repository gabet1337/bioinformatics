#include "phylib_reader.hpp"
#include <iostream>
using namespace std;

int main () {

  phylib_reader pr;
  dist_matrix dm;
  labels l;
  pr.read("phylib/example_slide4.phy",l,dm);
  for (int i = 0; i < l.size(); i++) {
    cout << l[i] << "\t";
    for (int j = 0; j < l.size(); j++) {
      cout << dm[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}
