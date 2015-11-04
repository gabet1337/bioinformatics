#include <iostream>
#include "rfdist.hpp"
using namespace std;

char* files[8] = {"trees/clustalo-rnj.newick", "trees/kalign-rnj.newick",
                  "trees/mafft-rnj.newick", "trees/muscle-rnj.newick",
                  "trees/clustalo-qt.newick", "trees/kalign-qt.newick",
                  "trees/mafft-qt.newick", "trees/muscle-qt.newick"};


int main() {

  rfdist rf;
  int dist[8][8];
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      cout << "Running on " << files[i] << " & " << files[j] << endl;
      dist[i][j] = rf.distance(files[i],files[j]);
    }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      cout << dist[i][j] << "\t";
    }
    cout << endl;
  }

  return 0;
}
