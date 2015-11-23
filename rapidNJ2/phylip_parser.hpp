#ifndef PHYLIP_PARSER
#define PHYLIP_PARSER
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>

class phylip_parser {
public:
  void parse(std::string file, std::vector<std::vector<double> > &dist, std::vector<std::string> &labels);
  void parse(std::string file, std::vector<std::vector<std::pair<double,int> > > &dist, std::vector<std::string> &labels);
};

void phylip_parser::parse(std::string file, std::vector<std::vector<double> > &dist, std::vector<std::string> &labels) {

  std::ifstream in(file.c_str());
  std::string line;

  int n;
  in >> n;
  
  labels.resize(n);
  dist.resize(n);

  for (int i = 0; i < n; i++) {
    dist[i].resize(n);
    in >> labels[i];
    for (int j = 0; j < n; j++) {
	in >> dist[i][j];
    }
  }
  
}

void phylip_parser::parse(std::string file, std::vector<std::vector<std::pair<double,int> > > &dist, std::vector<std::string> &labels) {

  std::ifstream in(file.c_str());
  std::string line;

  int n;
  in >> n;
  
  labels.resize(n);
  dist.resize(n);

  for (int i = 0; i < n; i++) {
    dist[i].resize(n);
    in >> labels[i];
    for (int j = 0; j < n; j++) {
      double d;
      in >> d;
      dist[i][j] = std::make_pair(d,j);
      //in >> dist[i][j];
    }
  }
  
}

#endif
