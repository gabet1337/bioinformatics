#ifndef PHYLIB_READER_HPP
#define PHYLIB_READER_HPP

#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
// #include <boost/spirit/include/qi.hpp>
// #include <boost/iostreams/device/mapped_file.hpp>

typedef double weight_t;
typedef std::vector<std::vector<weight_t> > dist_matrix;
typedef std::vector<std::string> labels;

class phylib_reader {

public:
  phylib_reader();
  ~phylib_reader();
  void read(const char *file, labels &l, dist_matrix &dm);
  void read_fast(const char *file, labels &l, dist_matrix &dm);
  void read_self(const char *file, labels &l, dist_matrix &dm);
  // void read_boost(const char *file, labels &l, dist_matrix &dm);
};

phylib_reader::phylib_reader() {}
phylib_reader::~phylib_reader() {};

void phylib_reader::read(const char *file, labels &l, dist_matrix &dm) {
  std::ios_base::sync_with_stdio(false);
  int BUFFER_SIZE = 65536;
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
  unsigned int N;
  fscanf(f, "%d\n", &N);
  posix_fadvise(fileno(f), 0, 0, POSIX_FADV_SEQUENTIAL);
  l.resize(N);
  dm.assign(N,std::vector<weight_t>(N));

  for (unsigned int i = 0; i < N; ++i) {
    fscanf(f,"%s ", buf);
    l[i] = std::string(buf);
    for (unsigned int j = 0; j < N; ++j) {
      fscanf(f,"%lf ", &dm[i][j]);
    }
  }
}

inline bool ws(char c) {
  return c == 9 || c == 32 || c == 10;
}

inline void skip_ws(char *data, unsigned int &tidx) {
  while (ws(data[tidx])) tidx++;
}

void phylib_reader::read_self(const char *file, labels &l, dist_matrix &dm) {
  struct stat stat_buf;
  stat(file, &stat_buf);
  int fd = open(file, O_RDONLY);
  char* data = (char*)mmap(0, stat_buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
  int N;
  char buffer[20];
  unsigned int tidx = 0, i = 0;
  skip_ws(data, tidx);
  while ( !ws(data[tidx]) ) { buffer[i++] = data[tidx++]; }
  buffer[i] = '\0';
  N = atoi(buffer);

  l.resize(N);
  dm.resize(N);

  for (unsigned int row = 0; row < N-1; ++row) {
    dm[row].resize(N);
    i = 0;
    skip_ws(data, tidx);
    while ( !ws(data[tidx]) ) { buffer[i++] = data[tidx++]; }
    buffer[i] = '\0';
    l[i] = std::string(buffer);
    for (unsigned int col = 0; col < N; ++col) {
      i = 0;
      skip_ws(data, tidx);
      while ( !ws(data[tidx]) ) { buffer[i++] = data[tidx++]; }
      buffer[i] = '\0';
      dm[row][col] = atof(buffer);
    }
  }
  dm[N-1].resize(N);
  for (unsigned int col = 0; col < N-1; ++col) {
    i = 0;
    skip_ws(data, tidx);
    while ( !ws(data[tidx]) ) { buffer[i++] = data[tidx++]; }
    buffer[i] = '\0';
    dm[N-1][col] = atof(buffer);
  }
  skip_ws(data,tidx);
  i = 0;
  while ( tidx < stat_buf.st_size/sizeof(char) && !ws(data[tidx]) ) { buffer[i++] = data[tidx++];}
  buffer[i] = '\0';
  dm[N-1][N-1] = atof(buffer);

  // exit(0);

  munmap(data, stat_buf.st_size);
}

// void phylib_reader::read_boost(const char *file, labels &l, dist_matrix &dm) {
//   std::string line;
//   std::ifstream f(file);
//   using namespace boost::spirit::qi;

//   std::string s = "1.2 3.5 4.5\n3.5 4.3 4.2";

//   int N;
//   // f >> N;
//   // getline(f,line);
//   N = 2;
//   l.resize(N);
//   dm.assign(N,std::vector<double>());
//   for (int i = 0; i < N; i++) {
//     getline(f, line);
//     phrase_parse(line.begin(), line.end(),
// 		 (double_ >> *(' ' >> double_)), '%', dm[i]);
//   }

//   for (int i = 0; i < dm.size(); i++) {
//     for (int j = 0; j < dm[i].size(); j++)
//       std::cout << dm[i][j] << " ";
//     std::cout << std::endl;
//   }
    
		   
// }

#endif
