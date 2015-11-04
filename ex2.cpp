#include <iostream>
#include "rfdist.hpp"
using namespace std;

char* files[8] = {"trees/clustalo_permuted-rnj.newick", "trees/kalign_permuted-rnj.newick",
                  "trees/mafft_permuted-rnj.newick", "trees/muscle_permuted-rnj.newick",
                  "trees/clustalo_permuted-qt.newick", "trees/kalign_permuted-qt.newick",
                  "trees/mafft_permuted-qt.newick", "trees/muscle_permuted-qt.newick"};


int main() {

  int dist[8][8];
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      rfdist rf;
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
