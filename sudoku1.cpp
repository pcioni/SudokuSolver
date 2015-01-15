#include <vector>
#include <set>
#include <iostream>
#include <math.h>
#include <cassert>
#include "sudoku.h"

using std::set;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

// ======================================================
// A program to solve very simple Sudoku puzzles
// ======================================================

int main() {

  int size;
  while (cin >> size) {
    // Read in the board size and create an empty board
    Sudoku board(size);

    // Read in and set the known positions in the board
    char c;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {      
        cin >> c;
        if (c != '.') {
          board.Set(i,j,c-'0');
        }
      }
    }
    
    // The iterative solving routine 
    while (1) {
      
      // Is the puzzle is already solved?
      if (board.IsSolved()) {
        cout << "\nPuzzle is solved:" << endl;
        board.Print();
        break;
      }
      
      // Is the puzzle impossible?
      if (board.Impossible()) {
        cout << "\nCannot solve this puzzle:" << endl;
        board.Print();
        break;
      }
      
      // Try to reduce the number of choices in one or more board cells
      // by propagating known values within each row, column, and quadrant
      int changed = 0;
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
          if (board.KnownValue(i,j))
            changed += board.Propagate(i,j);
        }
      }
      
      // If nothing changed this iteration, give up
      if (changed == 0) {
        cout << "\nPuzzle is too difficult for this solver!" << endl;
        board.Print();
        break;      
      }
    }
  }
}

// ======================================================

// Construct an empty board
Sudoku::Sudoku(int s) {
	// set the size
	size = s;
	quadrant_size = (int)sqrt(size);
	assert (quadrant_size*quadrant_size == s);
  
	std::set<int> full_set;
	for (int i=1; i <= size; ++i)
		full_set.insert(i);
 
	std::vector<std::set<int> > vectors;
	for (int j=0; j < size; ++j) 
		vectors.push_back(full_set);
	
	for (int k=0; k < size; ++k)
		board.push_back(vectors);
	
}


// To construct the puzzle, set the value of a particular cell
void Sudoku::Set(int i, int j, int value) {
  // make sure the value is one of the legal values
  assert (board[i][j].find(value) != board[i][j].end());
  board[i][j].clear();
  board[i][j].insert(value);
}

// Does the cell have exactly one legal value?
bool Sudoku::KnownValue(int i, int j) const {
  return (board[i][j].size() == 1);
}

// If there is exactly one number in each cell of the grid, the puzzle is solved!
bool Sudoku::IsSolved() const {
	for (int i=0;i<size;++i){
		for (int j=0;j<size;++j){
			if (board[i][j].size()!=1)
				return false;
		}
	}
	return true;
}

// If there are no legal choices in one or more cells of the grid, the puzzle is impossible
bool Sudoku::Impossible() const {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (board[i][j].size() == 0) 
        return true;
    }
  }
  return false;
}

// print out the solution
void Sudoku::Print() const {
  int solved = IsSolved();
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int count = size+1;
      for (set<int>::iterator itr = board[i][j].begin(); itr != board[i][j].end(); itr++) {
        count--;
        cout << *itr;
      }
      if (!solved) {
        // if it's not solved, print out spaces to separate the values
        for (int k = 0; k < count; k++) {
          cout << " ";
        }
      }
    }
    cout << endl;
  }
}


// Propagate information from this cell along columns & rows &
// within quadrant. Return the number of cells that changed.
int Sudoku::Propagate(int i, int j) {
	int changed = 0;
	std::set<int> contains = board[i][j];

	for (int rc = 0; rc < size; ++rc) {	
		std::set<int>::iterator it;
		for (it = contains.begin(); it != contains.end(); ++it) {
			if (rc != i)
				changed += board[rc][j].erase(*it);
			if (rc != j)
				changed += board[i][rc].erase(*it);
		}
	}

	int row = i / quadrant_size;
	int col = j / quadrant_size;
	
	int colxquad = col * quadrant_size;
	int rowxquad = row * quadrant_size;
	std::set<int>::iterator itr2;
	for (int r = rowxquad; r < rowxquad + quadrant_size; ++r) {
		for (int c = colxquad; c < colxquad + quadrant_size; ++c) {
			for (itr2 = contains.begin(); itr2 != contains.end(); ++itr2) {
				if (r != i && c != j)
					changed += board[r][c].erase(*itr2);
			}
		}
	}

  return changed;
}
