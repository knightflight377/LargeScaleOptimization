#include <iostream>
#include <vector>
#include "SparseRow.C"
using namespace std;

//This is the SparseMatrix class
//The matrix is represented as a vector that contains pointers to rows from the SparseRow class

//typedef std::list <std::pair <int, double > > *rowPtr;

class SparseMatrix {
	public:
	//Constructors--both the initial and the copy constructor
	SparseMatrix (int, double);
	SparseMatrix (const SparseMatrix &a);

	//Destructor
	~SparseMatrix();

	//Variables
	int rows; //Number of rows in matrix A
	int ncols; //Number of columns in matrix A
	//vector <double> b; //The vector of structural constraint values, pass this as an argument to solveSystem
	double t; //This is the zero tolerance
	vector < Row* > vectOfRowPointers; //The vector of Row pointers

	//Methods
	bool insertMatrixElement(int, int, double); //insert the given element into the specified position in A
	bool deleteMatrixElement(int, int); //delete the element at the given position
	bool interchangeRow(int, int); //Switch the given rows
	bool multiplyByScaler(int, double); //Multiply the given row by a scaler
	bool combineRows(int, int, double); //Replace row with itself + constant*second row
	//vector SolveSystem(vector); //Solves the linear system
	void printMatrix(); //Prints the matrix

};

//Constructor
SparseMatrix::SparseMatrix (int r, double tolerance) {
	if (r > 0) {
		//Initialize rows and t
		rows = r;
		t = tolerance;
		//Create a null Row pointer
		Row* rPtr = NULL;

		//Build the Row pointer vector
		for (int i = 0; i < rows; i++) {
			rPtr = new Row(rows, t);
			vectOfRowPointers.push_back(rPtr);
		}
	}
};

//Copy constructor
SparseMatrix:: SparseMatrix(const SparseMatrix& a) {
	rows = a.rows;
	//copy the vector of row pointers


};

//Destructor -- free up the used memory
SparseMatrix:: ~SparseMatrix() {
	//delete each of the rows (delete the lists of pairs)
	for (vector < Row* >::iterator it = vectOfRowPointers.begin(); it != vectOfRowPointers.end(); it++) {
		delete (*it);
	}
	//clear the vector Row pointers itself
	vectOfRowPointers.clear();
};

//Insert an element into the given matrix position
//r is the row index
//c is the column index
//v is the value to be inserted
bool SparseMatrix:: insertMatrixElement(int r, int c, double v) {
	//check that r is a valid row index
	if (r > 0 && r < rows-1) {
		//access the correct row and call the insertElement function from SparseRow.C
		(*vectOfRowPointers[r]).insertElement(c, v);
		return true;
	}
	else {
		return false;
	}
};

//Delete the element at the given matrix position
//r is the row index
//c is the column index
bool SparseMatrix:: deleteMatrixElement(int r, int c) {
	//check that r is a valid row index
	if (r > 0 && r < rows-1) {
		//access the correct row and call the deleteEntry function from SparseRow.C
		(*vectOfRowPointers[r]).deleteEntry(c);
		return true;
	}
	else {
		return false;
	}
}

//Multiply every value in a row by a scaler
bool SparseMatrix:: multiplyByScaler(int r, double scaler) {
	//check that r is a valid row index
	if (r > 0 && r < rows-1) {
		//access the correct row and call the multiplyRowByScaler function from SparseRow.C
		(*vectOfRowPointers[r]).multiplyRowByScaler(scaler);
		return true;
	}
	else {
		return false;
	}
};

//Swap the given rows
//r1 and r2 are row indices
bool SparseMatrix:: interchangeRow(int r1, int r2) {
	//check that r1 and r2 are valid row indices
	if (r1 > 0 && r1 < rows-1 && r2 > 0 && r2 < rows-1) {
		//use the built in swap method for vectors
		vectOfRowPointers[r1].swap(vectOfRowPointers[r2]);
		return true;
	}
	else {
		return false;
	}
};

//Multiply r1 with r1 + constant*r2
//r1 and r2 are row indices
bool SparseMatrix:: combineRows(int r1, int r2 , double constant) {

};

//Solve the linear system
//If no solution exists, return the empty vector
//vector SparseMatrix:: SolveSystem(vector b) {

//};

//Print the matrix
void SparseMatrix:: printMatrix() {
	for (int j = 0; j < rows; j++) {
		//use the print function from SparseRow.C
		(*vectOfRowPointers[j]).print();
		cout << endl;
	}
};

int main() {

	cout << "I got here" << endl;

};