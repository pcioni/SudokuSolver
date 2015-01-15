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
  
  // You need to finish this function!
  std::set<int> full_set;
  for (int i=1;i<size+1;++i){
    full_set.insert(i);
  }
  std::vector<std::set<int> > vector_sets;
  for (int j=0;j<size;++j){
    vector_sets.push_back(full_set);
  }
  for (int k=0;k<size;++k){
    board.push_back(vector_sets);
  }
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
  // You need to write this function
  for (int i=0;i<size;++i){
    for (int j=0;j<size;++j){
      if (board[i][j].size()!=1){
        return false;
      }
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


// Propagate information from this cell along columns & diagonals & <<-- ??
// within quadrant. Return the number of cells that changed.
int Sudoku::Propagate(int i, int j) {
  int changed = 0;
  std::set<int> contains = board[i][j];

  //rows & columns
  for (int rc = 0; rc <size; ++rc){
    std::set<int>::iterator it;
    for (it = contains.begin();it!=contains.end();++it){
      if (rc!=i){
        changed+=board[rc][j].erase(*it);
        //changed+=board[i][rc].erase(*it);
      }
    }
  }
  for (int rc2 = 0; rc2 < size; ++rc2){
    std::set<int>::iterator itr;
    for (itr = contains.begin();itr!=contains.end();++itr){
      if (rc2!=j){
        changed+=board[i][rc2].erase(*itr);
      }
    }
  }

  int row = (int)i/quadrant_size;
  int col = (int)j/quadrant_size;

  for (int r = row*quadrant_size; r<row*quadrant_size + quadrant_size; ++r){
    for (int c = col*quadrant_size; c<col*quadrant_size + quadrant_size; ++c){
      std::set<int>::iterator it2;
      for (it2 = contains.begin();it2!=contains.end();++it2){
        if (r!=i && c!=j){
          changed+=board[r][c].erase(*it2);
        }
      }
    }
  }



  return changed;

}




