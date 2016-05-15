OPTS = -Wall -g -std=c++11

SparseMatrixInputDriver: SparseMatrixInputDriver.o SparseMatrix.o
	g++ $(OPTS) -o SparseMatrixInputDriver SparseMatrixInputDriver

SparseMatrixInputDriver.o:	SparseMatrixInputDriver.cpp
	g++ $(OPTS) -c SparseMatrixInputDriver.cpp

SparseMatrixDriver:	SparseMatrixDriver.o SparseMatrix.o
	g++ $(OPTS) -o SparseMatrixDriver SparseMatrixDriver.o

SparseMatrixDriver.o:	SparseMatrixDriver.cpp
	g++ $(OPTS) -c SparseMatrixDriver.cpp

SparseMatrix:	SparseMatrix.o SparseRow.o
	g++ $(OPTS) -o SparseMatrix SparseMatrix.o

SparseMatrix.o:	SparseMatrix.cpp
	g++ $(OPTS) -c SparseMatrix.cpp
 
SparseRow:	SparseRow.o
	g++ $(OPTS) -o SparseRow SparseRow.o

SparseRow.o:	SparseRow.C
	g++ $(OPTS) -c SparseRow.C
