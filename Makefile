
LAL_DIR = ../linear-arrangement-library
a.out: asdf.cpp $(LAL_DIR)/lal-debug/liblaldebug.a
	g++ -std=c++17 -DDEBUG -D_GLIBCXX_DEBUG -o a.out asdf.cpp -I $(LAL_DIR) -L $(LAL_DIR)/lal-debug -llaldebug -lgmp -fopenmp
