#include "newick_parser.hpp"
#include "rfdist.hpp"
using namespace std;

int main(int argc, char *argv[]) {

  if (argc != 3) {
    //usage
    cout << "Usage: " << argv[0] << " <tree1> <tree2>" << endl;
    return 0;
  }

  rfdist rf;
  cout << rf.distance(argv[1], argv[2]) << endl;
  return 0;

}
