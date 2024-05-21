#include <string>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <stack>

//  John L. Sokol - May 21, 2012
//  RPN SpreadSheet Calculator
//
// 
//


using namespace std;

#define FALSE 0
#define TRUE 1


namespace Error
{

  struct CircularReferenceException
  {
  };

  struct Zero_divide
  {
  };

  struct Unresolved
  {
  };

  struct Table_error
  {
    const char *p;
      Table_error (const char *q)
    {
      p = q;
    }
  };

  struct Syntax_error
  {
    const char *p;
      Syntax_error (const char *q)
    {
      p = q;
    }
  };
}


class spreadsheet
{

  typedef struct
  {
    std::string data;
    double result;
    bool solved;
  } ElementType;

  typedef map < std::string, ElementType > TableType;

  TableType table;
  int x, y;


public:
  int unresolved_elements;
  bool tablefail;
  enum Token_value
  {
    STRING, NUMBER, PRINT, END, ADD = '+', SUB = '-', MUL = '*', DIV = '/'
  };


    spreadsheet ()
  {
  };

  ~spreadsheet ()
  {
  };

  double RPNeval (string expr);

  void load (istream &);

  void solve_pass ();
  void solve ();

  void dumpresults ();
  void dumpmap ();

};



double
spreadsheet::RPNeval (string expr)
{

  double a, b, result;

#define MATH_OPERATION( op ) \
	if(mystack.size() < 2) throw Error::Syntax_error("Stack Underrun"); \
	a = mystack.top(); mystack.pop(); \
	b = mystack.top(); mystack.pop(); \
	mystack.push(b op a);

  stringstream input (stringstream::in | stringstream::out);
  string token;

  stack < double >mystack;
  char ch;

  input << expr;

  while (input.get (ch))
    {
      switch (ch)
	{
	case 0:
	  break;		// Should never get here

	case '\t':
	case ' ':
	  break;

	case ADD:
	  MATH_OPERATION (+)break;
	case SUB:
	  MATH_OPERATION (-)break;
	case MUL:
	  MATH_OPERATION (*)break;
	case DIV:
	  if ((mystack.size () >= 2) && (mystack.top () == 0))
	    throw Error::Zero_divide ();
	  MATH_OPERATION (/)break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  input.putback (ch);
	  input >> result;
	  mystack.push (result);
	  break;

	default:
	  if (isalpha (ch))
	    {
	      input.putback (ch);
	      input >> token;

	      if (table.find (token.c_str ()) != table.end ())
		{
		  if (table[token.c_str ()].solved == 1)
		    {
		      mystack.push (table[token.c_str ()].result);
		    }
		  else
		    {
		      unresolved_elements++;
		      throw Error::Unresolved ();
		    }
		}
	      else
		throw Error::Syntax_error ("Invalid CELL");

	    }
	  else
	    throw Error::Syntax_error ("Invalid symbole");
	}

    }

  result = mystack.top ();
  mystack.pop ();
  return result;
}


void
spreadsheet::load (istream & input)
{
  char tmpstr[255];
  char element[6];
  int i, j;
  stringstream ss (stringstream::in | stringstream::out);

  unresolved_elements = 0;

  cin.getline (tmpstr, 255);
  ss << tmpstr;
  ss >> x;
  ss >> y;
// Check x & y size and throw exception for too large
  if (x > 26 || y > 100)
    {
      throw Error::Table_error ("exceeded maximum size");
      //     exit(1);
    }

  for (i = 0; i < x; i++)
    for (j = 0; j < y; j++)
      {
	sprintf (element, "%c%d", 'A' + i, 1 + j);

	cin.getline (tmpstr, 255);
	table[element].data = tmpstr;
	table[element].result = 0;
	table[element].solved = FALSE;
	unresolved_elements++;
      }
}



void
spreadsheet::solve_pass ()
{
  TableType::const_iterator iter;

  unresolved_elements = 0;
  tablefail = FALSE;

  for (iter = table.begin (); iter != table.end (); iter++)
    {
      try
      {
	if (table[iter->first].solved == FALSE)
	  {
	    table[iter->first].result = RPNeval (iter->second.data);
	    table[iter->first].solved = TRUE;
	  }
      }
      catch (Error::Unresolved)
      {
	// Don't do anything, will try again next passs. 
      }
      catch (Error::Syntax_error e)
      {
	cerr << "Syntax Error:" << e.p << "\n";
	tablefail = TRUE;
      }
      catch (Error::Zero_divide)
      {
	cerr << "Divide by Zero Error" << "\n";
	tablefail = TRUE;
      }
    }

}


void
spreadsheet::solve ()
{
  int lastunresolved;

  do
    {
      lastunresolved = unresolved_elements;
      //dumpresults();
      solve_pass ();
    }
  while ((unresolved_elements != 0)
	 && (unresolved_elements < lastunresolved) && !tablefail);

  if (unresolved_elements != 0 && !tablefail)
    throw Error::CircularReferenceException ();

  if (tablefail)
    throw Error::Syntax_error ("Unable to resolve table");



}

void
spreadsheet::dumpmap ()
{
  TableType::const_iterator iter;

  for (iter = table.begin (); iter != table.end (); iter++)
    {
      cout << iter->first << " = " << iter->second.data << "\n";

    }
}


void
spreadsheet::dumpresults ()
{
  TableType::const_iterator iter;
  cout << x << " " << y << "\n";

  for (iter = table.begin (); iter != table.end (); iter++)
    {
      //cout << iter->first 
      if (table[iter->first].solved)
	cout << table[iter->first].result ;
      else
	cout << "unresolved ";
      cout << "\n";
    }

}




int
main (int argc, char *argv[])
{

  spreadsheet ss;

  try
  {
    ss.load (cin);


    ss.solve ();

    if (ss.unresolved_elements == 0 && !ss.tablefail)
      ss.dumpresults ();

  }
  catch (Error::CircularReferenceException)
  {
    cerr << "Circular Reference Exception\n";
  }
  catch (Error::Syntax_error e)
  {
    cerr << "Syntax Error:" << e.p << "\n";
  }
  catch (Error::Table_error e)
  {
    cerr << "Table Error:" << e.p << "\n";
  }
  return 0;
}
