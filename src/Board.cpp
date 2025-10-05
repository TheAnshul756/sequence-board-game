#include "Board.h"
#include <iostream>

using namespace std;

Board::Board(IBoardLayoutStrategy* layoutStrategy) {
    layoutStrategy->initializeBoard(board);
    numRows = board.size();
    numCols = board[0].size();
}

void Board::placeChip(int row, int col, ChipType type) {
    board[row][col].placeChip(type);
}

void Board::removeChip(int row, int col) {
    board[row][col].placeChip(EMPTY);
}

SequenceResult Board::checkAndMarkSequences(ChipType type) {
    // Unmark all sequences first
    for(auto& row : board) {
        for(auto& cell : row) {
            if(cell.isInSequence() && cell.getChipType() == type) {
                cell.setInSequence(false);
            }
        }
    }
    SequenceResult result = {0, {}};
    // Check vertical sequences
    for(int i = 0; i < numCols; i++) {
        for(int j = 0; j < numRows - 4; j++) {
            bool allMatch = true;
            int sharedCells = 0;
            for(int k = 0; k < 5; k++) {
                if(!(board[j + k][i].getChipType() == type || board[j + k][i].isCornerCell())) {
                    allMatch = false;
                    break;
                }
                if(board[j + k][i].isInSequence()) sharedCells++;
            }
            if(allMatch && sharedCells <= 1) {
                vector<pair<int, int>> positions;
                for(int k = 0; k < 5; k++) {
                    if (!board[j + k][i].isCornerCell()) {
                        board[j + k][i].setInSequence(true);
                    }
                    positions.push_back({j + k, i});
                }
                result.count++;
                result.positions.push_back(positions);
                j += 4;
            }
        }
    }
    // Check horizontal sequences
    for(int i = 0; i < numRows; i++) {
        for(int j = 0; j < numCols - 4; j++) {
            bool allMatch = true;
            int sharedCells = 0;
            for(int k = 0; k < 5; k++) {
                if(!(board[i][j + k].getChipType() == type || board[i][j + k].isCornerCell())) {
                    allMatch = false;
                    break; 
                }
                if(board[i][j + k].isInSequence()) sharedCells++;
            }
            if(allMatch && sharedCells <= 1) {
                vector<pair<int, int>> positions;
                for(int k = 0; k < 5; k++) {
                    if (!board[i][j + k].isCornerCell()) {
                        board[i][j + k].setInSequence(true);
                    }
                    positions.push_back({i, j + k});
                }
                result.count++;
                result.positions.push_back(positions);
            }
        }
    }
    // check diagonal (top-left to bottom-right) sequences
    for(int i = 0; i < numRows - 4; i++) {
        for(int j = 0; j < numCols - 4; j++) {
            bool allMatch = true;
            int sharedCells = 0;
            for(int k = 0; k < 5; k++) {
                if(!(board[i + k][j + k].getChipType() == type || board[i + k][j + k].isCornerCell())) {
                    allMatch = false;
                    break;
                }
                if(board[i + k][j + k].isInSequence()) sharedCells++;
            }
            if (allMatch && sharedCells <= 1) {
                vector<pair<int, int>> positions;
                for(int k = 0; k < 5; k++) {
                    if (!board[i + k][j + k].isCornerCell()) {
                        board[i + k][j + k].setInSequence(true);
                    }
                    positions.push_back({i + k, j + k});
                }
                result.count++;
                result.positions.push_back(positions);
            }
        }
    }
    // check diagonal (top-right to bottom-left) sequences
    for(int i = 0; i < numRows - 4; i++) {
        for(int j = 4; j < numCols; j++) {
            bool allMatch = true;
            int sharedCells = 0;
            for(int k = 0; k < 5; k++) {
                if(!(board[i + k][j - k].getChipType() == type || board[i + k][j - k].isCornerCell())) {
                    allMatch = false;
                    break; 
                }
                if(board[i + k][j - k].isInSequence()) sharedCells++;
            }
            if (allMatch && sharedCells <= 1) {
                vector<pair<int, int>> positions;
                for(int k = 0; k < 5; k++) {
                    if (!board[i + k][j - k].isCornerCell()) {
                        board[i + k][j - k].setInSequence(true);
                    }
                    positions.push_back({i + k, j - k});
                }
                result.count++;
                result.positions.push_back(positions);
            }
        }
    }
    return result;
}

BoardCell Board::getCell(int row, int col) const {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) 
        throw out_of_range("Cell position out of range");
    return board[row][col];
}

bool Board::canPlaceChip(int row, int col, ChipType type) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) return false;
    BoardCell& cell = board[row][col];
    return cell.isEmpty() && !cell.isCornerCell();
}

bool Board::canRemoveChip(int row, int col, ChipType type) {
    if (row < 0 || row >= numRows || col < 0 || col >= numCols) return false;
    BoardCell& cell = board[row][col];
    return !cell.isEmpty() && cell.getChipType() != type && !cell.isInSequence();
}

int Board::getNumRows() const { return numRows; }

int Board::getNumCols() const { return numCols; }
