#include <iostream>
#include <vector>
#include <algorithm>
#include "SparseRow.C"
#include <cmath>
using namespace std;

//This is the SparseMatrix class
//The matrix is represented as a vector that contains pointers to rows from the SparseRow class
//This class also assumes that the matrix is a square matrix (# of rows = # of columns)

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
	vector <double> b; //The vector of structural constraint values, pass this as an argument to solveSystem
	double t; //This is the zero tolerance
	vector < Row* > vectOfRowPointers; //The vector of Row pointers

	//Methods
	bool insertMatrixElement(int, int, double); //insert the given element into the specified position in A
	bool deleteMatrixElement(int, int); //delete the element at the given position
	bool interchangeRow(int, int); //Switch the given rows
	bool multiplyByScaler(int, double); //Multiply the given row by a scaler
	bool combineRows(int, int, double); //Replace row with itself + constant*second row
	vector <double> SolveSystem(vector<double> &b); //Solves the linear system
	void printMatrix(); //Prints the matrix

	private:
	bool pivot(int , vector<double> &b); //Function that performs the pivot operation
	bool pivotGetZeros(int, vector<double> &b); //Function to get 0s in every row except the pivot row
};

//Constructor
SparseMatrix::SparseMatrix (int r, double tolerance) {
	if (r > 0) {
		//Initialize rows, ncols, and t
		rows = r;
		ncols = r;
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
	ncols = a.ncols;

	//Set up the vector with the correct length
	vectOfRowPointers.resize(rows);

	for (int m = 0; m < rows; m++) {
		//copy the pointers
		vectOfRowPointers[m] = a.vectOfRowPointers[m];
		//copy the rows the pointers point to
		Row *rTemp = new Row(*(a.vectOfRowPointers[m]));
		vectOfRowPointers[m] = rTemp;
	}
};

//Destructor -- free up the used memory
SparseMatrix:: ~SparseMatrix() {
	//delete each of the rows (delete the lists of pairs)
	for (vector < Row* >::iterator it = vectOfRowPointers.begin(); it != vectOfRowPointers.end(); it++) {
		delete (*it);
	}
	//clear the vector vectOfRowPointers itself
	vectOfRowPointers.clear();
};

//Insert an element into the given matrix position
//r is the row index
//c is the column index
//v is the value to be inserted
bool SparseMatrix:: insertMatrixElement(int r, int c, double v) {
	//check that r is a valid row index
	if (r >= 0 && r < rows) {
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
	if (r >= 0 && r < rows) {
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
	if (r >= 0 && r < rows) {
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
	if (r1 >= 0 && r1 < rows && r2 >= 0 && r2 < rows) {
		//classic swap
		Row* temp = vectOfRowPointers[r1];
		vectOfRowPointers[r1] = vectOfRowPointers[r2];
		vectOfRowPointers[r2] = temp;
		return true;
	}
	else {
		return false;
	}
};

//Multiply r1 with r1 + constant*r2
//r1 and r2 are row indices
bool SparseMatrix:: combineRows(int r1, int r2 , double constant) {
	//check that r1 and r2 are valid row indices
	if (r1 >= 0 && r1 < rows && r2 >= 0 && r2 < rows) {
		//get the rows and call replaceRow from SparseRow.C
		(*vectOfRowPointers[r1]).replaceRow((*vectOfRowPointers[r2]), constant);
		return true;
	}
	else {
		return false;
	}
};

//Solve the linear system
//Uses the pivot function and pivotGetZeros function
//If no solution exists, return the empty vector
vector <double> SparseMatrix:: SolveSystem(vector<double> &b) {
	bool returnValue;
	for (int i = 0; i < rows; i++) {
		bool returnValue = pivot(i, b);
		if (!returnValue) {
			cout << "There is no solution" << endl;
		}
		else {
			cout << "The solution is: " << b << endl;
		}
	}
};

//Print the matrix
void SparseMatrix:: printMatrix() {
	for (int j = 0; j < rows; j++) {
		//Get each row and set up an iterator
		SpRow rprint = (*vectOfRowPointers[j]).getRow();
		SpRow:: iterator it = rprint.begin();
		for (int m = 0; m < rows; m++) {
			//If the column index exists in the row, then the associated value is nonzero
			if ((*it).first == m) {
				cout << (*it).second << " ";
			}
			else {
				cout << "0" << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
};

//Use retrieveValue in this function
//This function finds the row with the greatest value at the given column index and pivots
//c is the column index
bool SparseMatrix:: pivot(int c, vector<double> &b) {
	double maxEntry = 0.0;
	int maxEntryRow = 0;

	for (int i = 0; i < rows; i++) {
		Row currentRow = (*vectOfRowPointers[i]);
		double temp = currentRow.retrieveValue(c);
		if (abs(temp) > maxEntry) {
			maxEntry = temp;
			maxEntryRow = i;
			interchangeRow(c, maxEntryRow);
			double scaler = 1.0/maxEntry;
			currentRow.multiplyRowByScaler(scaler);
			iter_swap(b.begin() + c, b.begin() + maxEntryRow);
			b[maxEntryRow] = b[maxEntryRow]*scaler;
		}
		else {
			return false;
		}
	}
	pivotGetZeros(c, b);
	return true;
};

//Old version of function, DELETE after confirming that new version works
// bool SparseMatrix:: pivot(int c, vector <double> b){
// 	double maxEntry = 0.0; //change this later to be the pivot point entry
// 	int maxEntryRow = 0; //change this later to be the pivot point row
// 	for (int i = 0; i < rows; i++) {
// 		Row currentRow = (*vectOfRowPointers[i]);
// 		SpRow::iterator it = currentRow.begin();
// 		//If the column index equals c and the absolute value of the value associated with index c
// 		//is greater than 0, swap the two rows in both the matrix and the vector b
// 		if ((*it).first == c && abs((*it).second) > maxEntry) {
// 			//set the max to the new absolute value
// 			max = abs((*it).second);
// 			//swap rows c and i to get the maximum value at the top
// 			interchangeRow(c, i);
// 			//set the scaler to 1/the max value
// 			double scaler = 1.0/(*it).second;
// 			//multiply the row by that scaler so that there's a 1 in position (c,c) of the matrix
// 			currentRow.multiplyRowByScaler(scaler);
// 			//do the same swapping and multiplication to the b vector
// 			iter_swap(b.begin() + c, b.begin() + i);
// 			b[i] = b[i]*scaler;
// 		}
// 		//If it's not possible to find an absolute value greater than 0, return false
// 		else {
// 			return false;
// 		}
// 	//return true;
// 	}
// 	//If the entire for loop is completed, call the pivotGetZeros function and return true;
// 	pivotGetZeros(c, vector <double> b);
// 	return true;
// };

//This function ensures that after the pivot is complete, there is a zero at column index c
// in every row except the pivot row
bool SparseMatrix:: pivotGetZeros(int c, vector <double> &b) {
	//Get the row that's being pivoted on, it will needed later
	Row pivotRow = (*vectOfRowPointers[c]);
	//Every other entry in the column must be a zero
	for (int i = 0; i < rows; i++) {
		//Get the current row
		Row currentRow = (*vectOfRowPointers[i]);
		if (i != c) {
			//We want to leave the pivot row alone, which is why (i != c)
			//We want to get the value at i and use it
			double constantScaler = currentRow.retrieveValue(i);
			//Do currentRow = currentRow + (-constantScaler)*pivotRow
			currentRow.replaceRow(pivotRow, -constantScaler);
			//Do the same operation to the b vector
			b[i] = b[i]*(-constantScaler);
		}
	}
	//If the entire for loop runs, return true
		return true;
	return false;
};

int main() {

	cout << "I got here" << endl;

	//Test of constructor
	SparseMatrix a(3, 0.1);
	cout << "Matrix a" << endl;
	a.printMatrix();
	cout << endl;

	//Test of insertMatrixElement
	cout << "Inserting into a" << endl;
	a.insertMatrixElement(2, 1, 2);
	a.insertMatrixElement(0, 1, 5);
	a.insertMatrixElement(1, 0, 4);
	a.printMatrix();
	cout << endl;

	//Test of multiplyByScaler
	cout << "Multplying by a scaler" << endl;
	a.multiplyByScaler(1, 3);
	a.printMatrix();
	cout << endl;

	//Test of interchangeRow
	cout << "Swapping rows" << endl;
	a.interchangeRow(1, 0);
	a.printMatrix();
	cout << endl;

	//Test of combineRows
	cout << "Replacing Rows" << endl;
	a.combineRows(1, 2, 3);
	a.printMatrix();

	//Test of deleteMatrixElement
	cout << "Deleting from a" << endl;
	a.deleteMatrixElement(1, 1);
	a.printMatrix();
	cout << endl;

	//Test of copy constructor
	cout << "Using copy constructor" << endl;
	SparseMatrix acpy(a);
	acpy.printMatrix();
	cout << endl;


};