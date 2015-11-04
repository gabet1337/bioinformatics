#include <iostream>
#include "rfdist.hpp"
using namespace std;

char* files[8] = {"trees/clustalo-rnj.newick", "trees/kalign-rnj.newick",
                  "trees/mafft-rnj.newick", "trees/muscle-rnj.newick",
                  "trees/clustalo-qt.newick", "trees/kalign-qt.newick",
                  "trees/mafft-qt.newick", "trees/muscle-qt.newick"};

char* files_permuted[8] = {"trees/clustalo_permuted-rnj.newick", "trees/kalign_permuted-rnj.newick",
                           "trees/mafft_permuted-rnj.newick", "trees/muscle_permuted-rnj.newick",
                           "trees/clustalo_permuted-qt.newick", "trees/kalign_permuted-qt.newick",
                           "trees/mafft_permuted-qt.newick", "trees/muscle_permuted-qt.newick"};


int main() {

  int dist[8];
  for (int i = 0; i < 8; i++) {
    rfdist rf;
    cout << "Running on " << files[i] << " & " << files_permuted[i] << endl;
    dist[i] = rf.distance(files[i],files_permuted[i]);
  }

  for (int i = 0; i < 8; i++) {
    cout << dist[i] << "\t";
  } cout << endl;

  return 0;
}
