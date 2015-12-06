CC = c++ -std=c++11 -O3 -g

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

ex5: newick_parser.hpp rfdist.hpp ex5.cpp test_lib.hpp
	$(CC) newick_parser.hpp rfdist.hpp test_lib.hpp ex5.cpp -o ex5

pr : phylib_reader.hpp phylib_reader_tester.cpp
	$(CC) phylib_reader.hpp phylib_reader_tester.cpp -o prt

njt : phylib_reader.hpp saitou_nei.hpp nj_tester.cpp newick_parser.hpp test_lib.hpp
	$(CC) -fopenmp test_lib.hpp newick_parser.hpp phylib_reader.hpp saitou_nei.hpp nj_tester.cpp -o nj

nj : phylib_reader.hpp saitou_nei.hpp nj.cpp newick_parser.hpp
	$(CC) -L/usr/lib/x86_64-linux-gnu/ -fopenmp newick_parser.hpp phylib_reader.hpp saitou_nei.hpp nj.cpp -o nj -lboost_iostreams

cpa : phylib_reader.hpp saitou_nei.hpp newick_parser.hpp test_lib.hpp rfdist.hpp compare_nj.cpp
	$(CC) -fopenmp phylib_reader.hpp saitou_nei.hpp newick_parser.hpp test_lib.hpp rfdist.hpp compare_nj.cpp -o cpa


crf : newick_parser.hpp rfdist.hpp compare_rfdist.cpp
	$(CC) newick_parser.hpp rfdist.hpp compare_rfdist.cpp -o crf

hpbf : hp_bf.cpp
	$(CC) hp_bf.cpp -o hpbf
