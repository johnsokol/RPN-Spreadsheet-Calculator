# RPN-Spreadsheet-Calculator
Reverse Polish Notation - Spreadsheet Calculator

https://johnsokol.blogspot.com/2014/06/reverse-polish-notation-spreadsheet.html

Spreadsheet Calculator 
A spreadsheet consists of a two-dimensional array of cells, labeled A1, A2, .... Each cell contains either an integer (its value) or an expression. 
Expressions contain integers, cell references, and the operators '+', '-', '*', '/'. Expressions are given in Reverse Polish Notation (see http://en.wikipedia.org/wiki/Reverse_Polish_notation). Write a program (in Python/C++/Java/C) to read a spreadsheet specification and evaluate the values of all the cells. A spreadsheet is specified as a plain text file:
• Line 1: two integers, defining the width and height of the spreadsheet (n, m) 
• n*m lines each containing an expression which is the value of the corresponding cell (cells enumerated in the order A1, A2, A, B1, ...). 
Your program should output its data in the same format, but each cell should be reduced to a single floating point value.  
Example 
For example, the following spreadsheet: 
2 2
A2
4 5 *
A1 B2 /
3
evaluates to: 
2 2
20
20
6.6666667
3
The above example input visually looks like:
1 2
-------------------------
A | A2 | 4 5 * |
-------------------------
B | A1 B2 / | 3 |
-------------------------
Test Cases
• We provide several example input and output files. • We value elegance, simplicity, and readability in your code 
Comments • All numbers in the input are positive integers, but internal calculations and output should be with floats. You can assume that there are no more than 26 rows (A-Z) in the spreadsheet. • Your program should detect cyclic dependencies in the input data and report these by throwing CircularReferenceException. 
• You may assume that input file is formatted correctly, and all of the expressions are well-formed. 
• Just passing the provided test cases does NOT mean you have a correct implementation.
