#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "SparseRow.C"
using namespace std;

//This is the SparseMatrix class
//The matrix is represented as a vector that contains pointers to rows from the SparseRow class
//This class also assumes that the matrix is a square matrix (# of rows = # of columns)

class SparseMatrix {
	public:
	//Constructors--both the initial and the copy constructor
	SparseMatrix (unsigned int, double);
	SparseMatrix (const SparseMatrix &a);

	//Destructor
	~SparseMatrix();

	//Variables
	unsigned int rows; //Number of rows in matrix A
	unsigned int ncols; //Number of columns in matrix A
	vector <double> b; //The vector of structural constraint values, pass this as an argument to solveSystem
	double t; //This is the zero tolerance
	vector < Row* > vectOfRowPointers; //The vector of Row pointers

	//Methods
	bool insertMatrixElement(unsigned int, unsigned int, double); //insert the given element into the specified position in A
	bool deleteMatrixElement(unsigned int, unsigned int); //delete the element at the given position
	bool interchangeRow(unsigned int, unsigned int); //Switch the given rows
	bool multiplyByScalar(unsigned int, double); //Multiply the given row by a scalar
	bool combineRows(unsigned int, unsigned int, double); //Replace row with itself + constant*second row
	vector <double> solveSystem(vector<double> &b); //Solves the linear system
	void printMatrix(); //Prints the matrix

	private:
	bool pivot(unsigned int , vector<double> &b); //Function that performs the pivot operation
	bool pivotGetZeros(unsigned int, vector<double> &b); //Function to get 0s in every row except the pivot row
};

//Constructor
SparseMatrix::SparseMatrix (unsigned int r, double tolerance) {
		//Initialize rows, ncols, and t
		rows = r;
		ncols = r;
		t = tolerance;

		//Create a null Row pointer
		Row* rPtr = NULL;

		//Build the Row pointer vector
		for (unsigned int i = 0; i < rows; i++) {
			rPtr = new Row(rows, t);
			vectOfRowPointers.push_back(rPtr);
		}
};

//Copy constructor
SparseMatrix:: SparseMatrix(const SparseMatrix& a) {
	rows = a.rows;
	ncols = a.ncols;

	//Set up the vector with the correct length
	vectOfRowPointers.resize(rows);

	for (unsigned int m = 0; m < rows; m++) {
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
bool SparseMatrix:: insertMatrixElement(unsigned int r, unsigned int c, double v) {
	//check that r is a valid row index
	if (r < rows) {
		//access the correct row and call the insertElement function from SparseRow.C
		vectOfRowPointers[r]->insertElement(c, v);
		return true;
	}
	else {
		return false;
	}
};

//Delete the element at the given matrix position
//r is the row index
//c is the column index
bool SparseMatrix:: deleteMatrixElement(unsigned int r, unsigned int c) {
	//check that r is a valid row index
	if (r < rows) {
		//access the correct row and call the deleteEntry function from SparseRow.C
		vectOfRowPointers[r]->deleteEntry(c);
		return true;
	}
	else {
		return false;
	}
}

//Multiply every value in a row by a scaler
bool SparseMatrix:: multiplyByScalar(unsigned int r, double scalar) {
	//check that r is a valid row index
	if (r < rows) {
		//access the correct row and call the multiplyRowByScaler function from SparseRow.C
		vectOfRowPointers[r]->multiplyRowByScalar(scalar);
		return true;
	}
	else {
		return false;
	}
};

//Swap the given rows
//r1 and r2 are row indices
bool SparseMatrix:: interchangeRow(unsigned int r1, unsigned int r2) {
	//check that r1 and r2 are valid row indices
	if (r1 < rows && r2 < rows) {
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
bool SparseMatrix:: combineRows(unsigned int r1, unsigned int r2 , double constant) {
	//check that r1 and r2 are valid row indices
	if (r1 < rows && r2 < rows) {
		//get the rows and call replaceRow from SparseRow.C
		vectOfRowPointers[r1]->replaceRow((*vectOfRowPointers[r2]), constant);
		return true;
	}
	else {
		return false;
	}
};

//Solve the linear system
//Uses the pivot function and pivotGetZeros function
//If no solution exists, return the empty vector
vector <double> SparseMatrix:: solveSystem(vector<double> &b) {
	for (int i = 0; i < rows; i++) {
		bool returnValue = pivot(i, b);
		if (!returnValue) {
			cout << "There is no solution" << endl;
		}
		else {
			cout << "The solution is: " << endl;
			//print out the b vector
			for (unsigned int i = 0; i < rows; i++) {
				cout << b[i] << endl;
			}
		}
	}
	return b;
};

//Print the matrix
void SparseMatrix:: printMatrix() {
	for (unsigned int j = 0; j < rows; j++) {
		//Get each row and set up an iterator
		SpRow rprint = vectOfRowPointers[j]->getRow();
		SpRow:: iterator it = rprint.begin();
		for (unsigned int m = 0; m < rows; m++) {
			//If the column index exists in the row, then the associated value is nonzero
			if (it != rprint.end() && it->first == m) {
				cout << it->second << " ";
				it++;
			}
			else {
				cout << "0" << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
};

//This function finds the row with the greatest value at the given column index and moves that row
//to the top of the matrix, then divides that row by a scaler so that the value at [c, c] is 1
//c is the column index
bool SparseMatrix:: pivot(unsigned int c, vector<double> &b) {
	double maxEntry = 0.0;
	int maxEntryRow = 0;
	Row* pivotRow = vectOfRowPointers[c];
	double tempScalar = 0.0;

	//Find the maximum entry in the given column
	for (unsigned int i = c; i < rows; i++) {
		Row& currentRow = (*vectOfRowPointers[i]);
		double temp = currentRow.retrieveValue(c);
		if (abs(temp) > maxEntry) {
			//Set temp to tempScaler, this will be used later for the scalar
			//You don't want to use the abs(temp) as tempScalar because then the multiplyRowByScalar operation
			//using scalar will have a -1 in the pivot position 
			tempScalar = temp;
			maxEntry = abs(temp);
			maxEntryRow = i;
		}
	}
	//Now that maxEntry is positive, check to see if it's within the tolerance
	//If it is, return false because that means that there's no way to get a 1 in the pivot point
	if (maxEntry < t) {
		return false;
	}
	else {
	//If the maxEntry is not 0, do the following to get a 1 in the pivot point
		interchangeRow(c, maxEntryRow);
		pivotRow = vectOfRowPointers[c];

		cout << "Pivot Row" << endl;
		pivotRow->print();
		double scalar = 1.0/tempScalar;
		pivotRow->multiplyRowByScalar(scalar);

		//Do the same swapping operation to right-hand side vector b
		iter_swap(b.begin() + c, b.begin() + maxEntryRow);

		//Do the same multiplying by scalar operation to right-hand side vector b
		b[c] = b[c]*scalar;

		cout << "Last matrix t in pivot function" << endl;
		printMatrix();

		cout << "Calling pivotGetZeros" << endl;
		//Call the pivotGetZeros function and return true since pivot was successful
		pivotGetZeros(c, b);
		return true;
	}
};

//This function ensures that after the pivot is complete, there is a zero at column index c
// in every row except the pivot row
bool SparseMatrix:: pivotGetZeros(unsigned int c, vector <double> &b) {
	//Get the row that's being pivoted on, it will needed later
	cout << "c is" << c << endl;
	Row* pivotRow = vectOfRowPointers[c];	

	cout << "pivotRow" << endl;
	pivotRow->print();
	cout << "Matrix before for loop" << endl;
	printMatrix();
	//Every other entry in the column must be a zero
	for (unsigned int i = 0; i < rows; i++) {
		cout << "i =" << " " << i << endl;
		//Get the current row
		Row* currentRow = vectOfRowPointers[i];
		if(i != c) {
			//We want to leave the pivot row alone, which is why (i != c)
			//We want to get the value at i and use it
			cout << "c currently is" << c << endl;
			double constantScalar = currentRow->retrieveValue(c);
			cout << "constantScalar from pivotGetZeros function"  << " " << constantScalar << endl;
			//Do currentRow = currentRow + (-constantScalar)*pivotRow
			currentRow->replaceRow(*pivotRow, -constantScalar);
			cout << "currentRow after the replaceRow operation in pivotGetZeros function" << endl;
			currentRow->print();
			cout << "Matrix t in pivotGetZeros function" << endl;
			printMatrix();
			//Do the same operation to the b vector
			b[i] = b[i] + b[c]*(-constantScalar);
		}
	}
	//If the entire for loop runs, return true
		return true;
	return false;
};