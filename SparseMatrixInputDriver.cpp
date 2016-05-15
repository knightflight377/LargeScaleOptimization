#include "SparseMatrix.cpp"
using namespace std;

int main() {

	int r;
	cout << "Please enter the number of rows and the tolerance";
	cin >> r >> t;
	if (r > 0) {
		SparseMatrix big(r, t);
	}

	int rr;
	int cc;
	double vv;
	cout "Please enter a matrix entry as row #, column #, and value, separated by a space";

	while (rr >= 0) {
		cin >> rr >> cc >> vv;
		big.insertMatrixElement(rr, cc, vv);
	}

	vector <double> rhs;
	int bb;

	cout << "Please enter a value for the right-hand side vector";
	while (bb < r) {
		cin >> bb;
		rhs.push_back(bb);
	}

	cout << "Matrix A" << endl;
	big.printMatrix();

	cout << endl;
	cout << "Input vector b" << endl;
	for (int i = 0; i < r; i++) {
		cout << rhs[i] << endl;
	}
	cout << "Now solving the given system" << endl;
	big.solveSystem(rhs);

	return 0;
};