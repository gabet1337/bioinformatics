#include "phylib_reader.hpp"
#include <iostream>
using namespace std;

int main () {

  phylib_reader pr;
  std::pair<labels, dist_matrix> res = pr.read("phylib/example_slide4.phy");
  for (int i = 0; i < res.first.size(); i++) {
    cout << res.first[i] << "\t";
    for (int j = 0; j < res.first.size(); j++) {
      cout << res.second[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}
