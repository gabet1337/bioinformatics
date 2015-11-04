CC = c++ -std=c++11 -O2 -g

parser: newick_parser.hpp newick_test.cpp
	$(CC) newick_parser.hpp newick_test.cpp -o newick_test

rfdist: newick_parser.hpp rfdist.cpp rfdist.hpp
	$(CC) newick_parser.hpp rfdist.hpp rfdist.cpp -o rfdist

ex1: newick_parser.hpp rfdist.hpp ex1.cpp
	$(CC) newick_parser.hpp rfdist.hpp ex1.cpp -o ex1

ex2: newick_parser.hpp rfdist.hpp ex2.cpp
	$(CC) newick_parser.hpp rfdist.hpp ex2.cpp -o ex2

ex3: newick_parser.hpp rfdist.hpp ex3.cpp
	$(CC) newick_parser.hpp rfdist.hpp ex3.cpp -o ex3
