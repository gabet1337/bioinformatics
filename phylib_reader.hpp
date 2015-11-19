#ifndef PHYLIB_READER_HPP
#define PHYLIB_READER_HPP

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <string.h>

typedef double weight_t;
typedef std::vector<std::vector<weight_t> > dist_matrix;
typedef std::vector<std::string> labels;

class phylib_reader {

public:
  phylib_reader();
  ~phylib_reader();
  void read(const char *file, labels &l, dist_matrix &dm);
  void read_fast(const char *file, labels &l, dist_matrix &dm);
};

phylib_reader::phylib_reader() {}
phylib_reader::~phylib_reader() {};

void phylib_reader::read(const char *file, labels &l, dist_matrix &dm) {
  std::ios_base::sync_with_stdio(false);
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

void phylib_reader::read_fast(const char *file, labels &l, dist_matrix &dm) {
  char buf[10];
  FILE* f = fopen(file, "r");
  int N;
  fscanf(f, "%d\n", &N);
  posix_fadvise(fileno(f), 0, 0, POSIX_FADV_SEQUENTIAL);
  l.resize(N);
  dm.assign(N,std::vector<weight_t>(N));

  for (size_t i = 0; i < N; i++) {
    fscanf(f,"%s ", buf);
    l[i] = std::string(buf);
    for (size_t j = 0; j < N; j++) {
      fscanf(f,"%lf ", &dm[i][j]);
    }
  }
}

#endif
