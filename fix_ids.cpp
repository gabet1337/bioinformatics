#include <iostream>
#include <string>
using namespace std;


int main() {
  
  int N;
  int next_id = 1;
  double w;
  string s;
  cin >> N;
  cout << N << std::endl;
  for (int i = 0; i < N; i++) {
    cin >> s; cout << next_id++ << " ";
    for (int j = 0; j < N; j++) {
      cin >> w;
      cout << w << " ";
    }
    cout << std::endl;
  }

  return 0;
}
