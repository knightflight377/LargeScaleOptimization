#include "SparseMatrix.cpp"

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
	cout << "Multplying by a scalar" << endl;
	a.multiplyByScalar(1, 3);
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

	//Test of SolveSystem
	cout << "Test of SolveSystem" << endl;
	SparseMatrix t(3, 0.01);
	//Set up row 0
	t.insertMatrixElement(0, 0, 2);
	t.insertMatrixElement(0, 1, 1);
	t.insertMatrixElement(0, 2, 3);
	//Set up row 1
	t.insertMatrixElement(1, 0, 2);
	t.insertMatrixElement(1, 1, 6);
	t.insertMatrixElement(1, 2, 8);
	//Set up row 2
	t.insertMatrixElement(2, 0, 6);
	t.insertMatrixElement(2, 1, 8);
	t.insertMatrixElement(2, 2, 18);
	cout << "Matrix t" << endl;
	t.printMatrix();
	cout << endl;

	//Set up the right-hand side vector
	vector <double> tt;
	tt.push_back(1);
	tt.push_back(3);
	tt.push_back(5);
	cout << "Right-hand side vector:" << endl;
	for (int i = 0; i < 3; i++) {
		cout << tt[i] << endl;
	}

	//Call SolveSystem; the answer should be [3/10, 2/5, 0]
	t.solveSystem(tt);

};