#include <iostream>
#include "rfdist.hpp"
#include <string>
using namespace std;

int sizes[14] = {89,214,304,401,494,608,777,877,
		 1347,1493,1560,1689,1756,1849};


int main() {
  rfdist r;
  for (int i = 0; i < 14; i++) {
    cout << sizes[i] << ": rnj vs my : " << r.distance( ("trees/rnj-"+to_string(sizes[i])+".new").c_str(), ("trees/MyNJ-"+to_string(sizes[i])+".new").c_str()) << endl;

    cout << sizes[i] << ": qt vs my : " << r.distance( ("trees/qt-"+to_string(sizes[i])+".new").c_str(), ("trees/MyNJ-"+to_string(sizes[i])+".new").c_str()) << endl;

    cout << sizes[i] << ": qt vs rnj : " << r.distance( ("trees/qt-"+to_string(sizes[i])+".new").c_str(), ("trees/rnj-"+to_string(sizes[i])+".new").c_str()) << endl;
    
  }


  return 0;
}
