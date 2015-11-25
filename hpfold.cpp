#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main(int argc, char *argv[]) {
  string S = string(argv[1]);
  int N = S.size();

  vector<int> evens,odds;
  for (int i = 0; i < N; i++)
    if (S[i] == 'h' && i%2==0) evens.push_back(i);
    else if (S[i] == 'h' && i%2==1) odds.push_back(i);

  for (int i = 0; i < evens.size(); i++)
    cout << evens[i] << " ";
  cout << endl;
  for (int i = 0; i < odds.size(); i++)
    cout << odds[i] << " ";
  cout << endl;
  vector<pair<int,int> > evens_odds,odds_evens;
  //match evens from the left with odds from the right:
  for (int i = 0, j = odds.size()-1; i < evens.size() && j >= 0 && evens[i] < odds[j]; i++,j--) {
    cout << "matched " << evens[i] << " with " << odds[j] << endl;
    evens_odds.push_back(make_pair(evens[i],odds[j]));
  }

  for (int i = 0, j = evens.size()-1; i < odds.size() && j >= 0 && odds[i] < evens[j]; i++,j--) {
    cout << "matched " << odds[i] << " with " << evens[j] << endl;
    odds_evens.push_back(make_pair(odds[i], evens[j]));
  }

  cout << "evens from left with odds from right: " << evens_odds.size() << endl;

  cout << "odds from left with evens from right: " << odds_evens.size() << endl;
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

  return 0;
}
