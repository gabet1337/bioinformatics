#include <iostream>
#include "saitou_nei.hpp"
#include "test_lib.hpp"
#include <fstream>
#include <string>
using namespace std;
string files[14] = {"phylib/89_Adeno_E3_CR1.phy"
		    ,"phylib/214_Arena_glycoprot.phy"
		    ,"phylib/304_A1_Propeptide.phy"
		    ,"phylib/401_DDE.phy"
		    ,"phylib/494_Astro_capsid.phy"
		    ,"phylib/608_Gemini_AL2.phy"
		    ,"phylib/777_Gemini_V1.phy"
		    ,"phylib/877_Glu_synthase.phy"
		    ,"phylib/1347_FAINT.phy"
		    ,"phylib/1493_Fe-ADH.phy"
		    ,"phylib/1560_Ferritin.phy"
		    ,"phylib/1689_FGGY_N.phy"
		    ,"phylib/1756_FAD_binding_3.phy"
		    ,"phylib/1849_FG-GAP.phy"};
int sizes[14] = {89,214,304,401,494,608,777,877,
		 1347,1493,1560,1689,1756,1849};

int main() {
  const int NUMBER_OF_RUNS = 1;
  test::clock c;
  ofstream qt_o("results/qt.dat");
  ofstream rnj_o("results/rnj.dat");
  ofstream sn_o("results/sn.dat");
  ofstream rnj2_o("results/rnj2.dat");
  for (int i = 0; i < 14; i++) {
    c.start();
    for (int j = 0; j < NUMBER_OF_RUNS; j++) {
      //system(("./quicktree -in m -out t " + files[i] + " > trees/qt-"+to_string(sizes[i])+".new").c_str());
    }
    c.stop();
    qt_o << sizes[i] << "\t" << c.count()/NUMBER_OF_RUNS << endl;

    c.start();
    for (int j = 0; j < NUMBER_OF_RUNS; j++) {
      //system(("./rapidnj -i pd -o t " + files[i] + " > trees/rnj-"+to_string(sizes[i])+".new").c_str());
    }
    c.stop();
    rnj_o << sizes[i] << "\t" << c.count()/NUMBER_OF_RUNS << endl;

    c.start();
    for (int j = 0; j < NUMBER_OF_RUNS; j++) {
      //system(("./nj " + files[i] + " > trees/MyNJ-"+to_string(sizes[i])+".new").c_str());
    }
    c.stop();
    sn_o << sizes[i] << "\t" << c.count()/NUMBER_OF_RUNS << endl;

     c.start();
    for (int j = 0; j < NUMBER_OF_RUNS; j++) {
      system(("./rapid " + files[i] + " > trees/Rapid2-"+to_string(sizes[i])+".new").c_str());
    }
    c.stop();
    rnj2_o << sizes[i] << "\t" << c.count()/NUMBER_OF_RUNS << endl;

    
  }
  
  return 0;
}
