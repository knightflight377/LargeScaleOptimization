#include "SparseRow.C"
using namespace std;

int main()
{
  Row r(20,0.1);
  Row s(35, 0.5);

  int col=1;
  double val;

  //Testing the insert function
  cout << "Inserting into row r" << endl;
  r.insertElement(2, 30);
  r.insertElement(3, 55);
  cout << "Row r contains:" << endl;
  r.print();

  cout << "Inserting into row s" << endl;
  s.insertElement(12, 6);
  s.insertElement(15, 4);
  cout << "Row s contains:" << endl;
  s.print();

  //Test of deleteEntry function
  cout << "Deleting entry in row r" << endl;
  r.deleteEntry(2);
  cout << "Row r is now:" << endl;
  r.print();

  //Test of multiplyRowByScaler function
  cout << "Now multiplying r by a scaler of 3" << endl;
  r.multiplyRowByScaler(3);
  cout << "Row r is now:" << endl;
  r.print();

  //Test of copy constructor
  Row rcp(r);
  cout << "Now printing rcp, which is a copy of r:" << endl;
  rcp.print();

  //Test of replaceRow function
  //DEBUG THIS, r still shows the state of r after the multiplyRowByScaler function, rather than the new values it should have
  r.replaceRow(s, 20);
  cout << "Now replacing row r" << endl;
  cout << "Row r is now:" << endl;
  r.print();
  r.replaceRow(s, 0.1);
  cout << "Replacing row r again" << endl;
  cout << "Now row r is:" << endl;
  r.print();

  cout << "Thanks for testing.  Always a good idea!" << endl;
};