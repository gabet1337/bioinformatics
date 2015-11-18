#ifndef POLYTREE_H
#define POLYTREE_H
#include "stdinclude.h"

class polytree{

 public:
  polytree(int size, vector<string>* sequenceNames);
  ~polytree(void);
  
  void addLeaf(std::string name);
  void addInternalNode(double left_dist, double right_dist, int left_index, int right_index);
  int getParentIndex(int child_index);
  int getLeftChildIndex(int parentIndex);
  int getRightChildIndex(int parentIndex);
  int getSibling(int index);
  void compareTreeBootstrap(polytree* tree);
  void serialize_tree(ostream &out);
  void serialize_node(ostream &out, unsigned int left_index, unsigned int right_index, int index);
  void set_serialization_indices(int left, int right, distType dist);
  int* bootstrap_counts;
  bool test;

 protected:
  unsigned int s_index_left;
  unsigned int s_index_right;
  void increaseLeafListsSize(vector<int>** leafList, unsigned int newMaxSize);  

 private:
  double *distances; //2n-1
  int *left_indexes;// n-1
  int *right_indexes;//n-1
  int *parent_indices; //2n-1
  std::string *leaf_names;//n
  unsigned int size;
  int current_index;
  char buffer[64];
  int leaf_index;  
  distType s_dist;
  void InitLeafLists(vector<int>** list);
  int bootstrap_replicate_count;
};

#endif
