#include <iostream>
#include <utility>
#include <list>
using namespace std;


//This class is the Row class used in SparseMatrix
//Each row is a list of pairs
//The first value of the pair is the column index and
//The second value of the pair is the nonzero entry

//The pair
typedef std::pair<unsigned int,double> SpRowEnt;
//The row
typedef std::list< SpRowEnt > SpRow;

class Row
{
private:
  unsigned int ncols;
  double tol;
  SpRow row;

public:

//Constructor
  Row(unsigned int numCols, double tolerance)
    : ncols(numCols), tol(tolerance), row() {}

//Print row
  void print() {
    SpRow::iterator it = row.begin();
    while (it != row.end()) {
	   cout << it->first << "  " << it->second << endl;
	   it++;
    }
  }

//Get the length of a row
  unsigned int lengthOfRow() {
    SpRow::iterator i = row.begin();
    unsigned int counter = 0;
    while (i != row.end()) {
      counter++;
      i++;
    }
    return counter;
  }

//Copy constructor  
//Copies the values from r into row
  Row(const Row &r) {

    ncols = r.ncols;
    tol = r.tol;
    row = SpRow();
    
    SpRow::const_iterator t = (r.row).begin();
    while (t != (r.row).end())
    {
      //need to use push_back method (from list class)
      row.push_back(*t);
      t++;
    }
  }

//Inserts an element, given a column index c and a value v
  bool insertElement(unsigned int c, double v) {

    //Invalid column index
    if (c >= ncols)
	   return false;  //insertion fails
    
    else {  
    //column index is valid
      SpRow::iterator it = row.begin();
        while (it != row.end()) {
          //Case 1: it points to a genuine row entry
	       if (it->first == c) { 
                //Subcase 1: delete this column if 'v' is effectively zero
	           if (v>= -tol && v<= tol) {
	               row.erase(it);
	               return true;
	           }
                //Subcase 2: v is not within the tolerance, so update it
	           else {
	               it->second = v;
	               return true;
	           }
	       }
           //Case 2: it is greater than c
	       if (it->first > c) {
                //we've made it past column c, we need to insert before it.
	            // only insert if 'v' is not effectively zero.
                if (v < -tol || v > tol) {
	               row.insert(it,SpRowEnt(c,v));
                }
	            return true;
	        }
	       //current column index is strictly less than 'c'
	       it++;
        }  //end of while loop
        // Case 3: reached end of list, insert new SpRowEnt here
        if (v < -tol || v > tol) {
            row.insert(it,SpRowEnt(c,v));
        }
        return true;
  } //end of else
}
  
//Deletes the entry at column index c
//Since deleting is essentially just inserting a 0, we call the insertElement function
  bool deleteEntry(unsigned int c) {
    return insertElement(c, 0.0);
  }

//Retrieve the value at a given column index c
  double retrieveValue(unsigned int c) {
    double entry;
    SpRow::iterator m = row.begin();

    while (m != row.end()) {
        //Case 1: If there's a column index that matches the given c, return the associated value
        if (m->first == c) {
            cout << "I'm in the if" << endl;
            entry = m->second;
            return entry;
        }
       //Case 2: The entry you're looking for is 0
        if (m->first > c) {
            return 0.0;
        } 
       //assert -- m->first < c
        m++;
    }
    //assert you've gone to the end of the row and didn't find the value you were looking for
    return 0.0;
}

//This function multiplies the row by a given scalar s
  bool multiplyRowByScalar(double s) {
    //If the scalar provided is within the tolerance, delete every row entry using the built-in clear function
    if (s >= -tol && s <= tol) {
      row.clear();
      return false;
    }
    //Otherwise, multiply every entry in the row by the scalar
    SpRow::iterator itt = row.begin();
    while (itt != row.end()) {
	   itt->second = s * itt->second;
	   itt++;
    }

    return true;
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

    SpRow otherRow = rI.getRow();
    SpRow::iterator i = otherRow.begin();
    SpRow::iterator n = row.begin();

    bool emptyRow = row.empty();
    bool emptyOtherRow = otherRow.empty();

    //If both rows are empty, there's no point in continuing
    if (emptyRow && emptyOtherRow) {
        return false;
    }

    //Otherwise, we can go ahead and do the replace row operation
    else {
	    while (i != otherRow.end() && n != row.end()) {

	      //Case 1: if the column value for rI is less than the column value for row, that means row has a 0 entry
	       if((i->first < n->first) || (n == row.end())) {
	           unsigned int columnnumber = i->first;
	           double value = con * i->second;

	        //If the newly calculated value is not within the tolerance, insert it
	           if (value < -tol || value > tol) {
	               row.insert(n, SpRowEnt(columnnumber, value));
	            }
	        i++;
	      }

	      //Case 2: if the column value for row is less than the column value rI, that means that rI has a 0 entry
	       if((n->first < i->first) || (i == otherRow.end())) {
	           n++;
            }

	      //Case 3: if the column values are the same
	       if(n->first == i->first) {
	           double nvalue = n->second + con * i->second;
	        //If the newly calculated value is not within the tolerance, replace row.second with it
	           if (nvalue < -tol || nvalue > tol) {
	               n->second = nvalue;
                    n++;
	            }
          //The nvalue is within the tolerance  
                else {
                    SpRow::iterator nn = n;
                    n++;
                    row.erase(nn);
                }
	           i++;
	        }  
        } //end of while loop
        return true;
    } //end of else
  }
};