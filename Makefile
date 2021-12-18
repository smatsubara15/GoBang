CXX=g++
CXXFLAGS= -Wall -g

gobang: gobang.o
	${CXX} gobang.o -o gobang

gobang.o: gobang.cpp
	${CXX} -c gobang.cpp