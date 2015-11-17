#ifndef PHYLIB_READER_HPP
#define PHYLIB_READER_HPP

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
typedef std::vector<std::vector<double> > dist_matrix;
typedef std::vector<std::string> labels;

class phylib_reader {

public:
  phylib_reader();
  ~phylib_reader();
  std::pair<labels, dist_matrix> read(const char *file);
};

phylib_reader::phylib_reader() {}
phylib_reader::~phylib_reader() {};

std::pair<labels,dist_matrix> phylib_reader::read(const char *file) {
  int BUFFER_SIZE = 16184;
  char buffer[BUFFER_SIZE];
  std::ifstream in_stream(file);
  in_stream.rdbuf()->pubsetbuf(buffer, BUFFER_SIZE);

  int N;
  in_stream >> N;

  labels l;
  l.resize(N);
  dist_matrix dm(N, std::vector<double>(N));
  for (int i = 0; i < N; i++) {
    in_stream >> l[i];
    for (int j = 0; j < N; j++) {
      in_stream >> dm[i][j];
    }
  }
  l.shrink_to_fit();
  return {l,dm};
}


#endif
