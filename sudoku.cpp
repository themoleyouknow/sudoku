#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iomanip>
#include "sudoku.h"
using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {

  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)

  cout << "Failed!" << '\n';
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
  assert(row == 9);
  in.close();
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << '\n';
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << '\n';
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char) ('A' + row) << " ";
  for (int i=0; i<9; i++) {
    cout << ( (i % 3) ? ':' : '|' ) << " ";
    cout << ( (data[i]=='.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << '\n';
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r) << "   ";
  cout << '\n';
  for (int r=0; r<9; r++) {
    print_frame(r);
    print_row(board[r],r);
  }
  print_frame(9);
}

/* add your functions here */

/* Define is_complete */
// Purpose: Returns boolean true if a the inputted sudoku board contains digits in all entries;
// returns false otherwise. 
bool is_complete(const char board[9][9]) {
  // Declare local variable 'success'
  bool success = true;
  //For loop to iterate across rows
  for (int row = 0; row < 9; row++) {
    //For loop to iterate across columns for each row
    for (int col = 0; col < 9; col++) {
      //Determine if success is still true
      success = isdigit(board[row][col]) && success;
      //If false, return false immediately
      if (success !=true) {
	return false;
      }
    }
  }
  //Return success if reached end
  return success;
}

/* Define make_move */
// Purpose: Returns boolean true if inputted "move string is" valid and makes the move;
// returns false otherwise.
bool make_move(const char move_str[], char digit, char board[9][9]) {
  // Check if move_str is the right size
  if (strlen(move_str)!=2) {
    cout << "Bad string was passed: " << move_str << " of length: " << strlen(move_str) << endl;
    return false;
  }
  // Declare and initialise row/column using ASCII table
  int row = int (move_str[0])-65;
  int col = int (move_str[1])-49;
  // Check if out of bounds
  if ( ((row>=9)||(row<0)) || ((col>=9)||(col<0)) ) {
    cout << "Bad row or Column number passed!" << endl;
    cout << "Attempted row: " << row << endl;
    cout << "Attempted column: " << col << endl;
    return false;
  }
  // Check if already contains a digit
  if (isdigit(board[row][col])) {
    return false;
  }
  // Make move
  board[row][col] = digit;
  // Return true 
  return true;
}

/* Define save_board */
// Purpose: Returns true if process of saving board is completed and saves board;
// returns false otherwise. 
bool save_board(const char* filename, char board[9][9]) {
  // Declare Stream
  ofstream out(filename);
  if (!out) return false;
  // Ensure outstream open
  assert(out);
  // For loop to run through rows of board
  for (int row = 0 ; row<9; row++) {
    // For loop to run through columns of board
    for (int col = 0 ; col<9; col++) {
      // Check to ensure outputting digits or '.', then output, else return false
      if (isdigit(board[row][col]) || board[row][col] == '.') {
	out.put(board[row][col]);
      }else return false;
    }
    // Put a line break after each row
    out.put('\n');
  }
  // Close stream
  out.close();
  // Return true
  return true;
}
// Define and Initialise Global Data Variable
int recursions = 0;

/* Define solve_board */
// Recursively solve a sudoku board, using helper function is_valid to solve the next valid cell.
// NB the row and column are initialised to 0 in the header file so that only a board needs to be
// used a parameter by the user.
bool solve_board( char board[9][9], int row, int col) {
  recursions++;
  // Declare and initialise trial number
  int num = 1;
  // Check if we've reached the end! 
  if (row>8) {
    cout <<  "Solution found, and number of recursions = " << recursions << endl;;
    recursions = 0;
    return true;
  }
  // Declare and convert row and col numbers
  char move_str[3];
  move_str[0] = char (row+65);
  move_str[1] = char (col+49);
  move_str[2] = '\0';
  // If loop to check if the space is blank
  if (!isdigit(board[row][col])) {
    // For loop to run through possible nums for this cell and test implications
    for ( ; num <=9; num++) {
    // Check if the move is valid
      if (is_valid(board, row, col, num) && make_move(move_str, char (num+48), board) ) {
	// Check next valid space
	if (solve_next(board, row, col)) return true;
	// Valid move, but no dice, change back to nothing
	board[row][col] = '.';
      }
    }
  }else {
    // Invalid move, move on to next unoccupied cell
    if (solve_next(board, row, col)) return true;
  }
  // No solution found! Return false
  if (row==0&&col==0) {
    cout << "No solution found, and number of recursions = " << recursions << endl;
    recursions = 0;

  }
  return false;
}

/* Define solve_next */
// Purpose: Determine whether we've reached the end of a row, and attempts to solve the next cell
// returning true if this is possible and false otherwise.
bool solve_next(char board[9][9], int row, int col) {
  // Check if we're at the final columns (left to right)
  if (col ==8 ) {
    // Try solving the first cell in the row below
    if (solve_board(board, row+1, 0)) return true;
  }else {
    // Try solving the next cell on this row
    if (solve_board(board, row, col+1)) return true;
  }
  // Return false if not possible
  return false;
}

/* Define is_valid */
// Purpose: Determine whether a number placed in a board at a particular row and column position
// is valid i.e. no other currently exists in the given row or column, and no other exists within
// the given sector.
bool is_valid(const char board[9][9], int row, int column, int num) {
    // Declare internal variables
    int counter = 0; 
    int sectorRow = 3*(row/3);
    int sectorCol = 3*(column/3);
    int row1 = (row+2)%3;
    int row2 = (row+4)%3;
    int col1 = (column+2)%3;
    int col2 = (column+4)%3;
    char trynum = num+48;
 
    // Check for the value in the given row and column
    for (counter=0; counter<9; counter++) {
      if (board[counter][column] == trynum) return false;
      if (board[row][counter] == trynum) return false;
    }
 
    // Check the remaining four spaces in the sector 
    if(board[row1+sectorRow][col1+sectorCol] == trynum) return false;
    if(board[row2+sectorRow][col1+sectorCol] == trynum) return false;
    if(board[row1+sectorRow][col2+sectorCol] == trynum) return false;
    if(board[row2+sectorRow][col2+sectorCol] == trynum) return false;
    return true;
}
