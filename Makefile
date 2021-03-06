OPTS = -Wall -g -std=c++11

# SparseMatrixInputDriver: SparseMatrixInputDriver.o SparseMatrix.o
# 	g++ $(OPTS) -o SparseMatrixInputDriver SparseMatrixInputDriver

# SparseMatrixInputDriver.o:	SparseMatrixInputDriver.cpp
# 	g++ $(OPTS) -c SparseMatrixInputDriver.cpp

SparseMatrixDriver:	SparseMatrixDriver.o SparseMatrix.o
	g++ $(OPTS) -o SparseMatrixDriver SparseMatrixDriver.o

SparseMatrixDriver.o:	SparseMatrixDriver.cpp
	g++ $(OPTS) -c SparseMatrixDriver.cpp

SparseMatrix:	SparseMatrix.o SparseRow.o
	g++ $(OPTS) -o SparseMatrix SparseMatrix.o

SparseMatrix.o:	SparseMatrix.cpp
	g++ $(OPTS) -c SparseMatrix.cpp

SparseRowDriver: SparseRowDriver.o SparseRow.o
	g++ $(OPTS) -o SparseRowDriver SparseRowDriver.o

SparseRowDriver.o: SparseRowDriver.cpp
	g++ $(OPTS) -c SparseRowDriver.cpp
	 
SparseRow:	SparseRow.o
	g++ $(OPTS) -o SparseRow SparseRow.o

SparseRow.o:	SparseRow.C
	g++ $(OPTS) -c SparseRow.C

clean:
	rm -f *.o
