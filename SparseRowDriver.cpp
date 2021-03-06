#include "SparseRow.C"
using namespace std;

int main()
{
  Row r(20,0.1);
  Row s(35, 0.5);

  //Testing the insert function
  cout << "Inserting into row r" << endl;
  r.insertElement(2, 3);
  r.insertElement(3, 5);
  cout << "Row r contains:" << endl;
  r.print();

  cout << "Inserting into row s" << endl;
  s.insertElement(12, 6);
  s.insertElement(15, 4);
  cout << "Row s contains:" << endl;
  s.print();

  //Test of retrieveValue function
  cout << "Now retreiving a value from r" << endl;
  r.retrieveValue(3);
  cout << "Value retrieved" << endl;

  //Test of replaceRow function
  //Row s should become (2, 60) (3, 100) (12, 6) (15, 4)
  s.replaceRow(r, 20);
  cout << "Now replacing S" << endl;
  cout << "Row s is now:" << endl;
  s.print();
  cout << "R is still:" << endl;
  r.print();

  //Test of deleteEntry function
  cout << "Deleting entry in row r" << endl;
  r.deleteEntry(2);
  cout << "Row r is now:" << endl;
  r.print();

  //Test of multiplyRowByScalar function
  //Row r should become (3, 15)
  cout << "Now multiplying r by a scaler of 3" << endl;
  r.multiplyRowByScalar(3);
  cout << "Row r is now:" << endl;
  r.print();

  //Test of copy constructor
  //Row rcp should be (3, 15)
  Row rcp(r);
  cout << "Now printing rcp, which is a copy of r:" << endl;
  rcp.print();

  cout << "Thanks for testing.  Always a good idea!" << endl;
};