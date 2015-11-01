//parses a file to an evolutionary tree
//nodes have a name and an adjacency list with double weights

#ifndef NEWICK_PARSER
#define NEWICK_PARSER
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>
#include <map>
#include <queue>
struct node {
  std::string name;
  std::vector<std::pair<node*,double> > adj_list;
  node() : name("") {}
};

typedef std::pair<std::string, std::pair<double,int> > sdi;
typedef std::pair<int,double> PID;
typedef std::vector<PID> vid;
typedef std::vector<vid> vvid;
class parser {
public:
  node* parse(std::string file);
  node* parse(char* file);
private:
  node* parsev(std::string x);
  sdi find_match(std::string x);
  std::vector<std::string> split(const std::string &s, char delim);
  std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
};

sdi parser::find_match(std::string x) {
  int depth = 1;
  int len = 1;
  int i = 1;
  for ( ; i < x.size(); i++,len++) {
    if (x[i] == ')') depth--;
    else if (x[i] == '(') depth++;
    if (depth == 0) break;
  }
  i++;
  std::string res = x.substr(0,i);
  std::string buffer = "0";
  if (x[i] == ':') {
    i++;len++;
    while (!(x[i] == ')' || x[i] == ',' || x[i] == ';')) {buffer+=x[i++]; len++;}
  }
  return std::make_pair(res, std::make_pair(std::stod(buffer), len));
}

node* parser::parse(std::string file) {
  return parse(file.c_str());
}

node* parser::parse(char* file) {
  std::ifstream t(file);
  std::string str;
  t.seekg(0, std::ios::end);
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);
  str.assign((std::istreambuf_iterator<char>(t)),
             std::istreambuf_iterator<char>());
  str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
  return parsev(find_match(str).first);  
}

node* parser::parsev(std::string x) {
  node* n = new node();
  for (int i = 1; i < x.size(); i++) {
    if (x[i] == '(') {
      sdi k = find_match(x.substr(i));
      node* c = parsev(k.first);
      c->adj_list.push_back(std::make_pair(n,k.second.first));
      // c->weight = k.second.first;
      n->adj_list.push_back(std::make_pair(c,k.second.first));
      i+=k.second.second+1;
    } else if (x[i] == ')') {
      // std::cout << "HERE@@@" << std::endl;
      if (i+1 == x.size()) continue;
      double w = std::stod(x.substr(i+2, x.size()-(i+2)));
      // n->weight = w;
    } else {
      std::string buffer;
      while (!(x[i] == ')' || x[i] ==',')) buffer+=x[i++];
      std::vector<std::string> s = split(buffer, ':');
      node* l = new node();
      l->name = s[0];
      // l->weight = std::stod(s[1]);
      double w = std::stod(s[1]);
      n->adj_list.push_back(std::make_pair(l,w));
      l->adj_list.push_back(std::make_pair(n,w));
    }
  }
  return n;
}

std::vector<std::string> &parser::split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::vector<std::string> parser::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}


#endif
