CC = c++ -std=c++11 -O2 -g

parser: newick_parser.hpp newick_test.cpp
	$(CC) newick_parser.hpp newick_test.cpp -o newick_test

rfdist: newick_parser.hpp rfdist.cpp
	$(CC) newick_parser.hpp rfdist.cpp -o rfdist
