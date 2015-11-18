#include "stdinclude.h"
#include "bootstrapper.hpp"
#include "rapidNJ.h"
#include "KimuraDistance.hpp"
#include "dataloaderMemory.hpp"
#include <time.h>
#include "bitStringUtils.hpp"
#include "dataloader.hpp"

bootstrapper::bootstrapper(dataloader* dl, bool fastDist, bool verbose, bool negative_branches) {
  //srand((unsigned)time(0));
  bootstrapper::sequenceCount = dl->getSequenceCount();
  bootstrapper::sequenceLength = dl->getSequenceLength();
  bootstrapper::originalSequences = *(dl->getSequences());
  bootstrapper::useFastDist = fastDist;
  bootstrapper::sequenceNames = dl->getSequenceNames();
  bootstrapper::verbose = verbose;
  bootstrapper::negative_branches = negative_branches;
  bootstrapper::inputType = dl->type;
  bootstrapper::newSequences.reserve(sequenceCount); 
  for(unsigned int i = 0 ; i < sequenceCount; i++){
    bootstrapper::newSequences.push_back(new char[sequenceLength]);    
  }
  bootstrapper::dataLoader = new dataloaderMemory(sequenceLength, sequenceCount, sequenceNames, inputType);
  if(fastDist){
    if(inputType == DNA){
      bitStringsCount = sequenceLength / 64 + 6;
      paddingLength = bitStringsCount*64 - sequenceLength;
      gapFilters = new unsigned int*[sequenceCount];
    } else {	
      bitStringsCount = sequenceLength / 16 + 8;
      paddingLength = bitStringsCount*16 - sequenceLength;
    }
    bitStrings = new unsigned int*[sequenceCount];
    for(unsigned int i = 0 ; i < sequenceCount; i++){
      //int retVal = posix_memalign((void**)&bitStrings[i],16,bitStringsCount*4*sizeof(unsigned int));
      bitStrings[i] = (unsigned int*) malloc_aligned(bitStringsCount*4*sizeof(unsigned int), 16);
      if(inputType == DNA){
        //int retVal = posix_memalign((void**)&gapFilters[i],16,bitStringsCount*4*sizeof(unsigned int));
        gapFilters[i] = (unsigned int*) malloc_aligned(bitStringsCount*4*sizeof(unsigned int), 16);
      }
    }
    if(inputType == DNA){
      encodeDNA(originalSequences);
    } else {
      encodeProtein(originalSequences);
    }
    dataLoader->initialize(bitStrings,gapFilters,bitStringsCount,paddingLength);
  } else {
    dataLoader->initialize(&originalSequences);
  }
}

void bootstrapper::encodeDNA(vector<char*>& sequences){
  for(unsigned int k = 0; k < sequenceCount; k++){
    for(unsigned int i = 0; i < sequenceLength; i++) {
      int offset = i % 16;
      unsigned int bitStringIdx = i / 16;
      if(offset == 0){	
        bitStrings[k][bitStringIdx] = 0;
        gapFilters[k][bitStringIdx] = 0;
      }
      char c = sequences[k][i];      
      //      cout << c << " ";
      switch (c) {
        //Nothing is done for gaps and ambigious/unknown nucleotides
      case 'A': 
      case 'a':
        //A has value 00 so nothing is added to the bitStrings
        gapFilters[k][bitStringIdx] += (Gbin << (offset*2));
        break;
      case 'C':
      case 'c':
        bitStrings[k][bitStringIdx] += (Cbin<<(offset*2));
        gapFilters[k][bitStringIdx] += (Gbin << (offset*2));
        break;      
      case 'G': 
      case 'g':
        bitStrings[k][bitStringIdx] += (Gbin<<(offset*2));
        gapFilters[k][bitStringIdx] += (Gbin << (offset*2));
        break;
      case 'T': 
      case 't':
        bitStrings[k][bitStringIdx] += (Tbin<<(offset*2));
        gapFilters[k][bitStringIdx] += (Gbin << (offset*2));
        break;      
      default:
        break;
      }
    }
    for(unsigned int i = sequenceLength; i < sequenceLength+paddingLength; i++) {
      int offset = i % 16;
      unsigned int bitStringIdx = i / 16;
      if(offset == 0){	
        bitStrings[k][bitStringIdx] = 0;
        gapFilters[k][bitStringIdx] = 0;
      }
    }
  }
}

void bootstrapper::encodeProtein(vector<char*>& sequences){
  for(unsigned int k = 0; k < sequenceCount; k++){
    for(unsigned int i = 0; i < sequenceLength; i++) {
      int offset = i % 4;
      unsigned int bitStringIdx = i / 4;
      if(offset == 0){
        bitStrings[k][bitStringIdx] = 0;
      }
      char c = sequences[k][i];
      //ignore positions with gaps and ambigious nucleotides
      if(c == '-' || c == '.' || c == 'X' || c == 'x' || c == 'z' || c == 'Z'  || c == 'b' || c == 'B' || c == 'J' || c == 'j' || c == '?') {
        bitStrings[k][bitStringIdx] += ('-' << (offset*8));
      } else {
        bitStrings[k][bitStringIdx] += (c << (offset*8));
      }
    }
    for(unsigned int i = sequenceLength; i < sequenceLength+paddingLength; i++) {
      int offset = i % 4;
      unsigned int bitStringIdx = i / 4;
      if(offset == 0){
        bitStrings[k][bitStringIdx] = 0;
      }
      bitStrings[k][bitStringIdx] += ('-' << (offset*8));
    }
  }
}

void bootstrapper::run(ostream &out, unsigned int replicates, unsigned int numThreads, ProgressBar* pb) {
  KimuraDistance* distanceCalculator = new KimuraDistance(verbose, useFastDist, dataLoader, false);
  distanceCalculator->computeDistances(numThreads);
  distType** distMatrix = distanceCalculator->getDistanceMatrix();  
  distMatrixReader* reader = new distMatrixReader(verbose, sequenceCount, false, sequenceNames, distMatrix);
  reader->initializeData();
  rapidNJ* nj = new rapidNJ(reader,sequenceCount, negative_branches, pb);  

  //compute first tree
  polytree* tree = nj->run();
  tree->serialize_tree(out);
  dataLoader->setSequences(&newSequences);
  //compute remaining trees
  for(unsigned int i = 0; i < replicates-1; i++){
    out << endl << endl;
    sampleSequences();
    if(useFastDist){
      if(inputType == DNA){
        encodeDNA(newSequences);
      } else {
        encodeProtein(newSequences);
      }
    }
    distanceCalculator->computeDistances(numThreads);
    polytree* tree = nj->run();
    tree->serialize_tree(out);
  } 
}

void bootstrapper::sampleSequences() {
  for(unsigned int i = 0; i < sequenceLength; i++){
    //select random column    
    unsigned int col = rand() % sequenceLength;
    for(unsigned int row = 0; row < sequenceCount; row++){
      newSequences[row][i] = originalSequences[row][col];
    }
  }
}
