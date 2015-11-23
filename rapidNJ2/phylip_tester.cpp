#include <iostream>
#include "phylip_parser.hpp"

using namespace std;

int main() {

  cout << "Parsing example_slide4.phy" << endl;

  std::vector<std::vector<double> > dist; // = new std::vector<std::vector<double> >();
  std::vector<string> labels;
  
  phylip_parser p;
  p.parse("example_slide4.phy", dist, labels);

  cout << dist[0][1] << endl;
  cout << labels[0] << endl;
  
  return 0;
  
}
