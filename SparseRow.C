#include <iostream>
#include <utility>
#include <list>
using namespace std;


//This class is the Row class used in SparseMatrix
//Each row is a list of pairs, where
//the first value of the pair is the column index and
//the second value of the pair is the nonzero entry

//The pair
typedef std::pair<int,double> SpRowEnt;
//The row
typedef std::list< SpRowEnt > SpRow;

class Row
{
private:
  int ncols;
  double tol;
  SpRow row;

public:

//Constructor
  Row(int numCols, double tolerance)
    : ncols(numCols), tol(tolerance), row() {}

//Print row
  void print() {
    SpRow::iterator it = row.begin();
    while (it != row.end()) {
	cout << (*it).first << "  " << (*it).second << endl;
	it++;
    }
  }

//Copy constructor  
//Copies the values from r into row
  Row(const Row &r) {

    ncols = r.ncols;
    tol = r.tol;
    row = SpRow();
    
    SpRow::iterator ii = row.begin();
    SpRow::const_iterator t = (r.row).begin();
    while (t != (r.row).end())
    {
      //need to use push_back method (from list class)
      row.push_back(*t);
      t++;
    }
  }

//Inserts an element, given a column index c and a value v
  bool insertElement(int c, double v) {

    //Invalid column index
    if ((c < 0) || (c >= ncols))
	   return false;  //insertion fails
    
    else {  
    //column index is valid
      SpRow::iterator it = row.begin();
      while (it != row.end()) {
        //it points to a genuine row entry
	     if ((*it).first == c)
	       { 
      //update this column
	      //delete this column if 'v' is effectively zero
	       if (v>= -tol && v<= tol) {
	       row.erase(it);
	       return true;
	       }
	     else {
	      //if v is not within the tolerance, update it
	       (*it).second = v;
	       return true;
	       }
	     }
	     if ((*it).first > c)
	       {
         //we've made it past column c, we need to insert before it.
	       // only insert if 'v' is not effectively zero.
	       row.insert(it,SpRowEnt(c,v));
	       return true;
	       }
	     //current column index is strictly less than 'c'
	     it++;
      }  //end of while loop
    //reached end of list, insert new SpRowEnt here
    row.insert(it,SpRowEnt(c,v));
    return true;
  }
}
  
//Deletes the entry at column index c
//Since deleting is essentially just inserting a 0, we call the insertElement function
  bool deleteEntry(int c) {
    return insertElement(c, 0.0);
  }

//This function multiplies the row by a given scaler s
  bool multiplyRowByScaler(double s) {

    SpRow::iterator itt = row.begin();
    //If the scaler provided is within the tolerance, delete every row entry using the built-in clear function
    if (s >= -tol && s <= tol) {
      while (itt != row.end()) {
        row.clear();
        itt++;
      }
      return false;
    }
    //Otherwise, multiply every entry in the row by the scaler
    else {
      while (itt != row.end()) {
	       (*itt).second = s * (*itt).second;
	       itt++;
      }
      return true;
    }
  }

//Gets the instance of row
  SpRow getRow() {

    return row;
  }

//Sets the instance row
  void setRow(SpRow r1) {

    row = r1;
  }

//This function replaces row with row + con*rI
//rI and row are rows
//Con is a constant
  bool replaceRow(Row& rI, double con) {

    SpRow::iterator i = rI.getRow().begin();
    SpRow::iterator n = row.begin();
    //If con is within the tolerance, do nothing since row will remain the same
    if (con >= -tol && con <= tol)
	   return true;
   //Otherwise, replacing rows is going to be needed
    else {
	   while (i != rI.getRow().end() && n != row.end()) {
	//If the column value for rI is less than the column value for row, that means row has a 0 entry
	     if(((*i).first < (*n).first) || (n == row.end())) {
	       int columnnumber = (*i).first;
	       double value = con * (*i).second;
	    //If the newly calculated value is not within the tolerance, insert it
	     if (value <= -tol || value >= tol) {
	       row.insert(n, SpRowEnt(columnnumber, value));
	     }
	     i++;
	   }
	  //If the column value for row is less than the column value rI, that means that rI has a 0 entry
	     if(((*n).first < (*i).first) || (i == rI.getRow().end()))
	       n++;
	  //If the column values are the same
	     if((*n).first == (*i).first) {
	       double nvalue = (*n).second + con * (*i).second;
	    //If the newly calculated value is not within the tolerance, replace row.second with it
	       if (nvalue <= -tol || nvalue >= tol) {
	         (*n).second = (*n).second + con * (*i).second;
	         }
	       i++;
	       n++;
	     }
      }
      return true;
    }
    //If the function finishes without doing either the initial if or one of the conditions inside the else, then return false
    return false;
  }
};
