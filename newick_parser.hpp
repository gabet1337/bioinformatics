//parses a file to an evolutionary tree
//nodes have a name, a child list and a parent pointer. Edges have weight

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
struct node {
  std::string name;
  std::vector<node*> adj_list;
  node* parent;
  double weight;
  node() : name(""), parent(0) {}
};

typedef std::pair<std::string, std::pair<double,int> > sdi;
class parser {
public:
  node* parse(std::string file);
  node* parsev(std::string x);
  sdi find_match(std::string x);
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

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

node* parser::parse(std::string file) {
  std::ifstream t(file.c_str());
  std::string str;
  t.seekg(0, std::ios::end);
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);
  str.assign((std::istreambuf_iterator<char>(t)),
             std::istreambuf_iterator<char>());
  str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
  return parsev(find_match(str).first);  
}


std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

node* parser::parsev(std::string x) {
  // std::cout << x << std::endl << std::endl;
  node* n = new node();
  for (int i = 1; i < x.size(); i++) {
    // std::cout << "char: " << x[i] << " at " << i << std::endl;
    if (x[i] == '(') {
      sdi k = find_match(x.substr(i));
      // std::cout << "STRING: " << k.first << std::endl
      //           << "WEIGHT: " << k.second.first << std::endl
      //           << "LENGTH: " << k.second.second << std::endl;
      node* c = parsev(k.first);
      c->weight = k.second.first;
      n->adj_list.push_back(c);
      i+=k.second.second+1;
    } else if (x[i] == ')') {
      //we are the root
      if (i+1 == x.size()) continue;
      //we have the weight from parent to this
      double w = std::stod(x.substr(i+2, x.size()-(i+2)));
      n->weight = w;
    } else {
      //we have a leaf situation!
      //read until , or ) and split on :
      // parse first part as the name and the second as the weight
      std::string buffer;
      while (!(x[i] == ')' || x[i] ==',')) buffer+=x[i++];
      // std::cout << "LEAF: " << buffer << std::endl;
      std::vector<std::string> s = split(buffer, ':');
      node* l = new node();
      l->name = s[0];
      l->weight = std::stod(s[1]);
      // std::cout << l->name << " " << l->weight << std::endl;
      n->adj_list.push_back(l);
    }
  }
  return n;
}

// node* parser::parse(const std::string &file) {
//   std::stack<node*> sta;
//   char c;
//   std::string buffer;
//   std::ifstream f(file.c_str());
//   node* root = new node();
//   node* cur = root;
//   sta.push(cur);
//   while (f >> c && c!=';') {
//     //new node - deeper level
//     if (c == '(') {
//       node* n = new node();
//       n->parent = cur;
//       sta.push(n);
//     } else if (c == ':') {
//       //edge
//       buffer = "";
//       while (f >> c && !(c == ',' || c == ')')) buffer+=c;

//     } else {
//       //leaf
//       buffer = "";
//       while (f >> c && !(c == ',' || c == ')')) buffer+=c;
//     }
//   }
// }

#endif
