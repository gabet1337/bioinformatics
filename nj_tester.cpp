#include <stdio.h>
#include <iostream>
#include "saitou_nei.hpp"
#include "test_lib.hpp"
using namespace std;

const char* files[14] = {"phylib/89_Adeno_E3_CR1.new"
			 ,"phylib/214_Arena_glycoprot.new"
			 ,"phylib/304_A1_Propeptide.new"
			 ,"phylib/401_DDE.new"
			 ,"phylib/494_Astro_capsid.new"
			 ,"phylib/608_Gemini_AL2.new"
			 ,"phylib/777_Gemini_V1.new"
			 ,"phylib/877_Glu_synthase.new"
			 ,"phylib/1347_FAINT.new"
			 ,"phylib/1493_Fe-ADH.new"
			 ,"phylib/1560_Ferritin.new"
			 ,"phylib/1689_FGGY_N.new"
			 ,"phylib/1756_FAD_binding_3.new"
			 ,"phylib/1849_FG-GAP.new"};
int sizes[14] = {89,214,304,401,494,608,777,877,
		 1347,1493,1560,1689,1756,1849};


int main(int argc, char *argv[]) {
  test::clock c;
  const int NUMBER_OF_RUNS = 1;
  for (int i = 0; i < 14; i++) {
    cout << "#Running on " << files[i] << endl;
    long long res = 0;
    for (int j = 0; j < NUMBER_OF_RUNS; j++) {
      c.start();
      saitou_nei sn(files[i]);
      std::string s = sn.compute();
      c.stop();
      res+=c.count();
    }
    cout << sizes[i] << "\t" << res/NUMBER_OF_RUNS << endl;
  }

  return 0;

}
