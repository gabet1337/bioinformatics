#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <random>
#include <climits>
#include <algorithm>
#define UNDEF 1000000
#define MAX_BF 20
using namespace std;
typedef pair<int,int> ii;

ii get_next_state(char c, ii &cp) {
  if (c == 'w') return {cp.first-1, cp.second};
  if (c == 'e') return {cp.first+1, cp.second};
  if (c == 'n') return {cp.first, cp.second+1};
  if (c == 's') return {cp.first, cp.second-1};
}

bool test_if_valid_path(vector<char> &path, map<ii, bool> &used, ii &pos, int idx) {
  used[pos] = true;
  ii ns = get_next_state(path[idx], pos);
  bool is_used = used.find(ns) != used.end();
  if (is_used) return false;
  if (idx == path.size()-1) return !is_used;
  return !is_used && test_if_valid_path(path,used,ns,idx+1);
}

int get_score_of_path(vector<char> &path, string &hp) {
  if (hp.size() <= 3) return 0;
  int score = 0;
  map<ii, char> miis;
  map<pair<ii,ii>, bool> used;
  ii prev,cur,next;
  prev = {UNDEF,UNDEF};
  cur = {0,0};
  next = get_next_state(path[0], cur);

  ii cf = cur;
  for (int i = 0; i < hp.size(); i++) {
    miis[cf] = hp[i];
    if (i != path.size())
      cf = get_next_state(path[i], cf);
  }

  prev = {UNDEF,UNDEF};
  cur = {0,0};
  next = get_next_state(path[0], cur);

  int dr[4] = {0,0,1,-1};
  int dc[4] = {1,-1,0,0};
  for (int i = 1; i <= hp.size(); i++) {
    for (int d = 0; d < 4; d++) {
      ii s = {cur.first+dr[d], cur.second+dc[d]};
      if (s==prev || s==next) continue;
      if (miis.find(s) != miis.end() &&
	  used.find({cur,s}) == used.end() &&
	  miis[s] == 'h' && miis[cur] == 'h') {
	--score;
	used[{cur,s}] = true;
	used[{s,cur}] = true;
      }
    }
    prev = cur;
    cur = next;
    if (hp.size() != i)
      next = get_next_state(path[i], next);
  }
  return score;
}

//translates n,s,w,e to f,l,r format!
vector<char> translate_nswe(vector<char> &path) {
  vector<char> result;
  result.push_back('f');
  for (int i = 1; i < path.size(); i++) {
    if (path[i] == path[i-1]) result.push_back('f');
    if (path[i-1] == 'n' && path[i] == 'w') result.push_back('l');
    if (path[i-1] == 'n' && path[i] == 'e') result.push_back('r');

    if (path[i-1] == 's' && path[i] == 'w') result.push_back('r');
    if (path[i-1] == 's' && path[i] == 'e') result.push_back('l');

    if (path[i-1] == 'e' && path[i] == 'n') result.push_back('l');
    if (path[i-1] == 'e' && path[i] == 's') result.push_back('r');

    if (path[i-1] == 'w' && path[i] == 's') result.push_back('l');
    if (path[i-1] == 'w' && path[i] == 'n') result.push_back('r');
  }
  return result;
}

vector<char> translate_flr(vector<char> &path) {
  vector<char> result;
  char current_dir = 'n';
  for (int i = 0; i < path.size(); i++) {
    if (path[i] == 'f') {}
    else if (path[i] == 'r' && current_dir == 'n') current_dir = 'e';
    else if (path[i] == 'r' && current_dir == 'e') current_dir = 's';
    else if (path[i] == 'r' && current_dir == 's') current_dir = 'w';
    else if (path[i] == 'r' && current_dir == 'w') current_dir = 'n';
    else if (path[i] == 'l' && current_dir == 'n') current_dir = 'w';
    else if (path[i] == 'l' && current_dir == 'e') current_dir = 'n';
    else if (path[i] == 'l' && current_dir == 's') current_dir = 'e';
    else if (path[i] == 'l' && current_dir == 'w') current_dir = 's';
    result.push_back(current_dir);
  }
  return result;
}

vector<char> convert_to_b3(unsigned long long n, int length) {
  vector<char> result(length,0);
  int idx = length-1;
  while (n > 0) {
    result[idx--] = n%3;
    n /= 3;
  }
  // while (result.size() < length) result.push_back(0);
  return result;
}

map<ii, bool> visited;
void dfs(ii cur, vector<char> &path, int n) {
  // cout << cur.first << " " << cur.second << endl;
  if (path.size() >= n) return;
  visited[cur] = true;
  vector<char> perm({'n','w','e','s'});
  random_shuffle(perm.begin(), perm.end());
  ii ns;
  for (int i = 0; i < 4; i++) {
    ns = get_next_state(perm[i], cur);
    if (visited[ns]) continue;
    path.push_back(perm[i]);
    dfs(ns, path, n);
    if (path.size() != n)
      path.pop_back();
  }
  visited[cur] = false;
}

int main(int argc, char *argv[]) {

  if (!(argc == 2 || argc == 3)) {
    cout << "Usage: " << argv[0] << " <hp-string> [optimal-score]" << endl;
    return 0;
  }
  
  string hp = string(argv[1]);
  int OPT_SCORE = UNDEF;
  if (argc == 3) OPT_SCORE = atoi(argv[2]);
  int N = hp.size();
  char alphabet[3] = {'f','l','r'};
  vector<char> best_path;
  for (int i = 0; i < N-1; i++) best_path.push_back('f');
  int best_score = 0;
  if (N <= MAX_BF) {
    cout << "TRY ALL COMBINATIONS!" << endl;
    //try all combinations!
    for (unsigned long long i = 0; i < pow(3,N-1); i++) {
      if (i%1000000 == 0) {
	if (best_score == OPT_SCORE) break;
	cout << (double)i/(double)pow(3,N-1)*100.0 << "% done" << endl;
	cout << "best so far " << best_score << endl;
      }

      vector<char> base3 = convert_to_b3(i,N-1);

      vector<char> path;
      for (int i = 0; i < N-1; i++) {
	path.push_back(alphabet[base3[i]]);
      }
      path = translate_flr(path);
      map<ii,bool> used;
      ii start = {0,0};
      if (test_if_valid_path(path, used, start, 0)) {
	int sc = get_score_of_path(path,hp);
	if (sc < best_score) {
	  cout << "NEW BEST: ";
	  cout << string(path.begin(), path.end()) << " " << sc << endl;
	  best_score = sc;
	  swap(best_path, path);
	}
      }
    }
  } else {
    cout << "Sample random valid paths and pick best" << endl;
    for (unsigned long long i = 0; i < pow(2,24); i++) {
      if (i%1000000 == 0) {
	if (best_score == OPT_SCORE) break;
	cout << (double)i/(double)pow(2,24)*100.0 << "% done" << endl;
	cout << "best so far " << best_score << endl;
      }
      vector<char> path;
      visited.clear();
      ii start = {0,0};
      dfs(start,path,N-1);
      map<ii,bool> used;
      int sc = get_score_of_path(path,hp);
      if (sc < best_score) {
	cout << string(path.begin(), path.end()) << endl;
	best_score = sc;
	swap(best_path, path);
      }
    }
  }
  string bp = string(best_path.begin(), best_path.end());
  cout << bp << endl;
  cout << "SCORE: " << best_score << endl;
  string prog = "./hpview.py " + hp + " " + bp;
  int x = system(prog.c_str());

  return 0;
}
