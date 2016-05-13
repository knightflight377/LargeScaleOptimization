OPTS = -Wall -g -std=c++11

SparseMatrix:	SparseMatrix.o SparseRow.o
	g++ $(OPTS) -o SparseMatrix SparseMatrix.o

SparseMatrix.o:	SparseMatrix.cpp
	g++ $(OPTS) -c SparseMatrix.cpp
 
SparseRow:	SparseRow.o
	g++ $(OPTS) -o SparseRow SparseRow.o

SparseRow.o:	SparseRow.C
	g++ $(OPTS) -c SparseRow.C
