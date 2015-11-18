#ifndef BOOTSTRAPPER_H_
#define BOOTSTRAPPER_H_

#include "dataloaderMemory.hpp"
#include "stdinclude.h"

class bootstrapper {

public:
  bootstrapper(dataloader* dl, bool fastDist, bool verbose, bool negative_branches);
  void run(ostream &out, unsigned int replicates, unsigned int numThreads, ProgressBar* pb);
  ~bootstrapper();
    
 private:  
  vector<char*> originalSequences;
  vector<char*> newSequences;
  vector<string>* sequenceNames;
  unsigned int sequenceLength;
  unsigned int sequenceCount;
  bool useFastDist;
  bool verbose;
  bool negative_branches;
  InputType inputType;
  unsigned int bitStringsCount;  
  dataloaderMemory* dataLoader;
  unsigned int** bitStrings;
  unsigned int** gapFilters;
  int paddingLength;
  
  void sampleSequences();
  void runFastDist(unsigned int replicates, unsigned int numThreads);
  void runNormal(unsigned int replicates, unsigned int numThreads);
  void encodeDNA(vector<char*>& sequences);
  void encodeProtein(vector<char*>& sequences);
};

#endif

