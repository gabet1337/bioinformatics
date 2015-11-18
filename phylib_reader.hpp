#ifndef PHYLIB_READER_HPP
#define PHYLIB_READER_HPP

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
typedef double weight_t;
typedef std::vector<std::vector<weight_t> > dist_matrix;
typedef std::vector<std::string> labels;

class phylib_reader {

public:
  phylib_reader();
  ~phylib_reader();
  void read(const char *file, labels &l, dist_matrix &dm);
};

phylib_reader::phylib_reader() {}
phylib_reader::~phylib_reader() {};

void phylib_reader::read(const char *file, labels &l, dist_matrix &dm) {
  int BUFFER_SIZE = 16384;
  char buffer[BUFFER_SIZE];
  std::ifstream in_stream(file);
  in_stream.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);

  int N;
  in_stream >> N;

  l.resize(N);
  dm.assign(N,std::vector<weight_t>(N));
  for (int i = 0; i < N; i++) {
    in_stream >> l[i];
    for (int j = 0; j < N; j++) {
      in_stream >> dm[i][j];
    }
  }
}


#endif
