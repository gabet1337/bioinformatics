#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
int main(int argc, char *argv[]) {

  string S = string(argv[1]);
  int N = S.size();

  vector<int> evens,odds;

  for (int i = 0; i < N; i++)
    if (S[i] == 'h' && i%2==0) evens.push_back(i);
    else if (S[i] == 'h' && i%2==1) odds.push_back(i);

  // cout << "e: ";
  // for (int i = 0; i < evens.size(); i++)
  //   cout << evens[i] << " ";
  // cout << endl;

  // cout << "o: ";
  // for (int i = 0; i < odds.size(); i++)
  //   cout << odds[i] << " ";
  // cout << endl;

  vector<pair<int,int> > evens_odds,odds_evens;

  int e1_odds, e2_odds, e1_evens, e2_evens;
  int e1_idx_odds, e2_idx_odds, e1_idx_evens, e2_idx_evens;

  //match evens from the left with odds from the right:
  for (int i = 0, j = odds.size()-1; i < evens.size() && j >= 0 && evens[i] < odds[j]; i++,j--) {
    //cout << "matched " << evens[i] << " with " << odds[j] << endl;
    evens_odds.push_back(make_pair(evens[i],odds[j]));
    e1_evens = evens[i];
    e1_idx_evens = i;
    e2_evens = odds[j];
    e2_idx_evens = j;
  }

  for (int i = 0, j = evens.size()-1; i < odds.size() && j >= 0 && odds[i] < evens[j]; i++,j--) {
    //cout << "matched " << odds[i] << " with " << evens[j] << endl;
    odds_evens.push_back(make_pair(odds[i], evens[j]));
    e1_odds = odds[i];
    e1_idx_odds = i;
    e2_odds = evens[j];
    e2_idx_odds = j;
  }

  int s1_odds, s2_odds, s1_evens, s2_evens;

  s1_odds = e1_odds + ((e2_odds - e1_odds) / 2);
  s2_odds = e2_odds - ((e2_odds - e1_odds) / 2);

  s1_evens = e1_evens + ((e2_evens - e1_evens) / 2);
  s2_evens = e2_evens - ((e2_evens - e1_evens) / 2);
  
  // cout << "evens: " << evens_odds.size() << ", e1_idx: " << e1_idx_evens << ", e2_idx: " << e2_idx_evens << endl;  

  // cout << "odds: " << odds_evens.size() << ", e1_idx: " << e1_idx_odds << ", e2_idx: " << e2_idx_odds << endl;

  vector<char> p1, p2;

  if (odds_evens.size() >= evens_odds.size()) {

    // cout << "GOT HERE: 1" << endl;

    // cout << "s1_odds: " << s1_odds << endl;
    
    // construct path 1
    for (int i=odds[e1_idx_odds]; i < s1_odds; i++) {
      p1.push_back('f');
    }

    for (int i=e1_idx_odds; i > 0; i--) {
      int low = odds[i-1];
      int high = odds[i];
      int med = (high - low) / 2;
      //cout << "i: " << i << ", low=" << low << ", high=" << high << ", med=" << med << endl;
      for (int j=low; j < high; j++) {
	if (med > 1 && (j == low || j == high-1)) {
	  p1.push_back('l');
	} else if (med > 1 && (j == low+med || j == low+med-1)) {
	  p1.push_back('r');
	} else {
	  p1.push_back('f');
	}
      }
    }

    for (int k=0; k < odds[0]; k++) {
      p1.push_back('f');
    }

    reverse(p1.begin(),p1.end());

    p1.push_back('r');
    p1.push_back('r');

    // cout << "s2_odds: " << s2_odds << endl;
    
    // construct path 2
    for (int i=s2_odds+1; i < evens[e2_idx_odds]; i++) {
      p2.push_back('f');
    }

    for (int i=e2_idx_odds; i < evens.size()-1; i++) {
      int low = evens[i];
      int high = evens[i+1];
      int med = (high - low) / 2;
      //cout << "i: " << i << ", low=" << low << ", high=" << high << ", med=" << med << endl;
      for (int j=low; j < high; j++) {
	if (med > 1 && (j == low || j == high-1)) {
	  p2.push_back('l');
	} else if (med > 1 && (j == low+med || j == low+med-1)) {
	  p2.push_back('r');
	} else if (j < N-2) {
	  p2.push_back('f');
	}
      }
    }

    for (int k=0; k < N-evens[evens.size()-1]-1; k++) {
      p2.push_back('f');
    }

  } else {

    // cout << "Got here: 2" << endl;
    
    //cout << "e1_idx_evens: " << e1_idx_evens << endl;
    //cout << "s1_evens: " << s1_evens << endl;


    // cout << "s1_evens: " << s1_evens << endl;
    // construct path 1
    for (int i=evens[e1_idx_evens]; i < s1_evens; i++) {
      p1.push_back('f');
    }

    for (int i=e1_idx_evens; i > 0; i--) {
      int low = evens[i-1];
      int high = evens[i];
      int med = (high - low) / 2;
      for (int j=low; j < high; j++) {
	if (med > 1 && (j == low || j == high-1)) {
	  p1.push_back('l');
	} else if (med > 1 && (j == low+med || j == low+med-1)) {
	  p1.push_back('r');
	} else {
	  p1.push_back('f');
	}
      }
    }
    
    for (int k=0; k < evens[0]; k++) {
      p1.push_back('f');
    }

    reverse(p1.begin(),p1.end());

    p1.push_back('r');
    p1.push_back('r');
    
    //cout << "s2_evens: " << s2_evens << endl;
    //cout << "e2_idx_evens: " << odds[e2_idx_evens] << endl;

    cout << "s2_evens: " << s2_evens << endl;
    
    // construct path 2
    for (int i=s2_evens+1; i < odds[e2_idx_evens]; i++) {
      p2.push_back('f');
    }

    cout << e2_idx_evens << " to " << odds.size() << endl;
    for (int i=e2_idx_evens; i < odds.size()-1; i++) {
      int low = odds[i];
      int high = odds[i+1];
      int med = (high - low) / 2;
      // cout << "i: " << i << ", low=" << low << ", high=" << high << ", med=" << med << endl;
      for (int j=low; j < high; j++) {
	if (med > 1 && (j == low || j == high-1)) {
	  p2.push_back('l');
	} else if (med > 1 && (j == low+med || j == low+med-1)) {
	  p2.push_back('r');
	} else if (j < N-2) {
	  p2.push_back('f');
	}
      }
    }

    for (int k=0; k < N-odds[odds.size()-1]-1; k++) {
      p2.push_back('f');
    }
    
  }

  for (int i=0; i<p1.size(); i++) {
    cout << p1[i];
  }

  for (int i=0; i<p2.size(); i++) {
    if (p1.size()+i < N-1)
      cout << p2[i];
  }
  if (N != p1.size()+p2.size()) {
    for (int i=0; i < (N-p1.size()-p2.size()-1); i++) {
      cout << "f";
    }
  }
  
  cout << endl;
  /*
  bool swapped = false;
  if (evens_odds.size() < odds_evens.size()) {
    swap(evens_odds, odds_evens);
    swapped = true;
  }
  //continue with evens_odds since it is largest:
  cout << evens_odds.size() << endl;

  //split into S1 and S2;
  string S1,S2;
  if (!swapped) {
    for (int i = 0, j = 0; i < N && j < evens_odds.size(); i++) {
      if (S[i] == 'h' && i%2==0) j++;
      S1+=S[i];
    }
    for (int i = S1.size(); i < S.size(); i++) S2+=S[i];
  } else {
    for (int i = 0, j = 0; i < N && j < evens_odds.size(); i++) {
      if (S[i] == 'h' && i%2==1) j++;
      S1+=S[i];
    }
    for (int i = S1.size(); i < S.size(); i++) S2+=S[i];
  }
  cout << S1 << " " << S1.size() << endl;
  cout << S2 << " " << S2.size() << endl;

  //fold S1 and S2 such that every other grid point is occopied by a matched h

  reverse(S2.begin(), S2.end());
  */
  
  return 0;
}
