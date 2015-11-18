/*This class is a very efficient implementation af a bifurcating phylogenetic tree.
* It can be serialised to Newick format. 
*/

#include "stdinclude.h"
#include "polytree.h"

using namespace std;

/* 
 * size - number of leafs
*/
polytree::polytree(int size, vector<string>* sequenceNames){
  distances = new double[size*2];
  left_indexes = new int[size];
  right_indexes = new int[size];  
  leaf_names = new string[size];
  polytree::size = size;
  parent_indices = new int[size*2];
  bootstrap_counts = new int[size];
  bootstrap_replicate_count = 0;
  test = false;
  for (int i = 0; i < size*2; i++) {
    parent_indices[i] = -1;
    if(i < size) {
      bootstrap_counts[i] = 0;
    }
  }
  current_index = 0;
  leaf_index = 0;  
  if(sequenceNames != NULL) {
    for(int k = 0; k < size; k++){
      addLeaf(sequenceNames->at(k));
    }
  }
}

void polytree::addLeaf(string name){
  leaf_names[leaf_index] = name;
  leaf_index++;
}

void polytree::addInternalNode(double left_dist, double right_dist, int left_index, int right_index){  
  left_indexes[current_index] = left_index;
  right_indexes[current_index] = right_index;    
  distances[left_index] = left_dist;
  distances[right_index] = right_dist;
  parent_indices[left_index] = current_index + size;
  parent_indices[right_index] = current_index + size;
  //cout << "left=" << left_index << " right=" << right_index << " parent=" << current_index + size << endl;
  current_index++;
}

int polytree::getParentIndex(int child_index){
  return parent_indices[child_index];
}

int polytree::getLeftChildIndex(int parentIndex) {
  return left_indexes[parentIndex - size];
}

int polytree::getRightChildIndex(int parentIndex) {  
  return right_indexes[parentIndex - size];
}

int polytree::getSibling(int index) {
  int parent = getParentIndex(index);
  if(parent == -1){
    return -1;
  }
  if(getLeftChildIndex(parent) == index) {
    return getRightChildIndex(parent);
  } else {
    return getLeftChildIndex(parent);
  }
}

void polytree::InitLeafLists(vector<int>** list) {
  for(unsigned int i = 0; i < size*2; i++) {
    list[i] = NULL;
  }
  for(unsigned int i = 0; i < size; i++) {    
    list[i] = new vector<int>();    
    list[i]->push_back(i);
  }
}

vector<int>* combineLeafLists(vector<int>* l1, vector<int>* l2) {
  vector<int>* retVal = new vector<int>;
  unsigned int idx1 = 0;
  unsigned int idx2 = 0;
  while(retVal->size() < l1->size() + l2->size()) {
    if(idx1 == l1->size()) {
      retVal->push_back(l2->at(idx2));
      idx2++;
    } else if(idx2 == l2->size()) {
      retVal->push_back(l1->at(idx1));
      idx1++;
    }
    else if(l1->at(idx1) < l2->at(idx2)) {
      retVal->push_back(l1->at(idx1));
      idx1++;
    } else {
      retVal->push_back(l2->at(idx2));
      idx2++;
    }
  }  
  return retVal;
}

void polytree::increaseLeafListsSize(vector<int>** leafLists, unsigned int newMaxSize) {
  for(unsigned int i = 0; i < size*2; i++) {
    if(leafLists[i] == NULL) {
      continue;
    }
    int parent = getParentIndex(i);
    if(parent == -1) {
      continue;
    }
    int sibling = getSibling(i);
    if(leafLists[sibling] != NULL) {
      if(leafLists[i]->size() + leafLists[sibling]->size() <= newMaxSize) {
        //Combine them
        vector<int>* newList = combineLeafLists(leafLists[i], leafLists[sibling]);
        leafLists[parent] = newList;
        delete leafLists[i];
        leafLists[i] = NULL;
        delete leafLists[sibling];
        leafLists[sibling] = NULL;
      }
    }
  }
}

int isEqualLists(vector<int>* l1, vector<int>* l2) {
  if(l1 == NULL || l2 == NULL) {
    return 0;
  }
  if(l1->size() != l2->size()) {
    return 0;
  }
  for(unsigned int i = 0; i < l1->size(); i++) {
    if(l1->at(i) != l2->at(i)) {
      return 0;
    } 
  }
  return 1;
}

void polytree::compareTreeBootstrap(polytree* tree) {
  bootstrap_replicate_count++;
  if(tree->size != size) {
    cerr << "Cannot compare trees with different sizes" << endl;
    exit(1);
  }
 /* vector<int>* testlist = new vector<int>();   
  testlist->push_back(121);
  testlist->push_back(128);
  testlist->push_back(147);
  testlist->push_back(148);
  testlist->push_back(149);*/

  int* nodeMap = new int[size*2];
  for(unsigned int i = 0; i < size*2; i++) {
    if(i < size) {
      nodeMap[i] = i;
    } else {
      nodeMap[i] = -1;
    }    
  } 
  vector<int>** leafLists = new vector<int>*[size*2];
  vector<int>** leafListsOther = new vector<int>*[size*2];
  InitLeafLists(leafLists);
  InitLeafLists(leafListsOther);
  for(unsigned int curSize = 2; curSize < size; curSize++) {    
    increaseLeafListsSize(leafLists, curSize);
    tree->increaseLeafListsSize(leafListsOther, curSize);

    /*if(test) {
      int found = false;
      int otherIdx = -1;
      for(int k = size; k < size*2; k++) {        
        if(leafListsOther[k] != NULL && leafListsOther[k]->size() == 2) {          
          if(isEqualLists(leafListsOther[k], testlist)) {
            cout << "HEP" << endl;
            found = true;
            otherIdx = k;
          }
        }
      }
      if(!found) {
        for(int k = size; k < size*2; k++) {        
          if(leafListsOther[k] != NULL) {
            for(int l = 0; l < leafListsOther[k]->size(); l++){
              if(leafListsOther[k]->at(l) == 121 || leafListsOther[k]->at(l) == 128) {
                cout << "FOUND: " << curSize << " ";
                for(int m = 0; m < leafListsOther[k]->size(); m++){
                  cout << leafListsOther[k]->at(m) << " ";
                }
                cout << endl;
              }
            }            
          }
        }
      }
      for(int k = size; k < size*2; k++) {
        if(leafLists[k] != NULL && leafLists[k]->size() == 2) {
          if(isEqualLists(leafLists[k],testlist)) {
            if(found && nodeMap[k] == -1) {
              cout << k << " other: " << otherIdx << " HEP" << endl;
            }
          }
        }
      }
    }*/

    for(unsigned int i = size; i < size*2; i++) {
       /* if(test && curSize == 2 && i == 343) {
        cout << "HEP" << endl;
     }*/

      vector<int>* list = leafLists[i];
      if(list == NULL || list->size() != curSize) {
        continue;
      }
      //Optimisation - look at known neighbor nodes first and then remaining nodes
      int child = getLeftChildIndex(i);
      int otherChild = nodeMap[child];
      if(otherChild != -1) {
        int otherParent = tree->getParentIndex(otherChild);
        if(otherParent != -1 && isEqualLists(list, leafListsOther[otherParent])) {
          nodeMap[i] = otherParent;          
        }
      }
      if(nodeMap[i] == -1) {
        //search all other nodes iteratively
        for(unsigned int j = size; j < size*2; j++) {

          /*if(test && curSize == 5 && j == 340) {
            cout << "HEP" << endl;
          }*/

          vector<int>* listOther = leafListsOther[j];
          if(listOther == NULL || listOther->size() != curSize) {
            continue;
          }   
          if(isEqualLists(list, listOther)) {
            nodeMap[i] = j;
            break;
          }
        }
      }
      if(nodeMap[i] != -1) {
        //found match
        bootstrap_counts[i-size]++;
      }
    }
  }
  vector<int>* list;
  for(unsigned int i = 0; i < size*2; i++) {
    list = leafLists[i];
    if(list != NULL) {
      delete list;
    }
    list = leafListsOther[i];
    if(list != NULL) {
      delete list;
    }
  }
  delete[] leafLists;
  delete[] leafListsOther;
  delete[] nodeMap;
  /*for(int i = 0; i < size; i++) {
    cout << i << ": " << leaf_names[i] << endl;
  }*/
}

void polytree::serialize_tree(ostream &out){  
  out << "(";
  if(s_index_left < size){
    out << "'" << leaf_names[s_index_left] << "'";
    out << ":";
    int length = sprintf(buffer,"%.5g", s_dist);
    string s = "";
    s.append(buffer,length);
    out << s;
  } else {
    if(s_index_right >= size){
      out << "(";
    }
    serialize_node(out, left_indexes[s_index_left - size] , right_indexes[s_index_left - size], s_index_left);
    if(s_index_right >= size){
      out << ")";
      if(bootstrap_replicate_count > 0) {        
        int bs = (int) (((double)bootstrap_counts[s_index_left-size] / (double) bootstrap_replicate_count) * 100);
        out << bs;
      }
      out << ":";
      int length = sprintf(buffer,"%.5g", s_dist);
      string s = "";
      s.append(buffer,length);
      out << s;
    }
  }
  out << ",";
  if(s_index_right < size){
    out << "'" << leaf_names[s_index_right] << "'";
    out << ":";
    int length = sprintf(buffer,"%.5g", s_dist);
    string s = "";
    s.append(buffer,length);
    out << s;
  } else { 
    serialize_node(out, left_indexes[s_index_right - size], right_indexes[s_index_right - size],s_index_right);    
  }
  out << ")";
  if(bootstrap_replicate_count > 0) {
    int bs = (int) (((double)bootstrap_counts[s_index_right-size] / (double) bootstrap_replicate_count) * 100);
    out << bs;
  }
  out << ";" << endl;
}

void polytree::serialize_node(ostream &out, unsigned int left_index, unsigned int right_index, int index){
  if(left_index < size){
    // leaf node
    out << "'" << leaf_names[left_index] << "'";
    out << ":";
    int length = sprintf(buffer,"%.5g", distances[left_index]);
    string s = "";
    s.append(buffer,length);
    out << s;
  } else {
    // serialize the left tree recursively
    out << "(";
    serialize_node(out, left_indexes[left_index - size], right_indexes[left_index-size], left_index);
    out << ")";
    if(bootstrap_replicate_count > 0) {
      int bs = (int) (((double)bootstrap_counts[left_index-size] / (double) bootstrap_replicate_count) * 100);
      out << bs;
    }
    out << ":";
    int length = sprintf(buffer,"%.5g", distances[left_index]);
    string s = "";
    s.append(buffer,length);
    out << s;
  }
  out << ",";
  if(right_index < size){
    // leaf node
    out << "'" << leaf_names[right_index] << "'";
    out << ":";
    int length = sprintf(buffer,"%.5g", distances[right_index]);
    string s = "";
    s.append(buffer,length);
    out << s;
  }
  else{
    // serialize the right tree recursively
    out << "(";
    serialize_node(out, left_indexes[right_index - size], right_indexes[right_index-size], right_index);
    out << ")";
    if(bootstrap_replicate_count > 0) {
      int bs = (int) (((double)bootstrap_counts[right_index-size] / (double) bootstrap_replicate_count) * 100);
      out << bs;      
    }
    out << ":";
    int length = sprintf(buffer,"%.5g", distances[right_index]);
    string s = "";
    s.append(buffer,length);
    out << s;
  }
}

void polytree::set_serialization_indices(int left, int right, distType dist) {
  s_index_left = left;
  s_index_right = right;
  s_dist = dist;
}

polytree::~polytree(void){
  delete[] distances;
  delete[] left_indexes;
  delete[] right_indexes;
  delete[] leaf_names;
  delete[] parent_indices;
  delete[] bootstrap_counts;
}
