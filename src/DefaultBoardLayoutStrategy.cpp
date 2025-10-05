#include "DefaultBoardLayoutStrategy.h"

using namespace std;

DefaultBoardLayoutStrategy::DefaultBoardLayoutStrategy() {
    boardNotation = {
        {"JJ", "6D", "7D", "8D", "9D", "TD", "QD", "KD", "AD", "JJ"},
        {"5D", "3H", "2H", "2S", "3S", "4S", "5S", "6S", "7S", "AC"},
        {"4D", "4H", "KD", "AD", "AC", "KC", "QC", "TC", "8S", "KC"},
        {"3D", "5H", "QD", "QH", "TH", "9H", "8H", "9C", "9S", "QC"},
        {"2D", "6H", "TD", "KH", "3H", "2H", "7H", "8C", "TS", "TC"},
        {"AS", "7H", "9D", "AH", "4H", "5H", "6H", "7C", "QS", "9C"},
        {"KS", "8H", "8D", "2C", "3C", "4C", "5C", "6C", "KS", "8C"},
        {"QS", "9H", "7D", "6D", "5D", "4D", "3D", "2D", "AS", "7C"},
        {"TS", "TH", "QH", "KH", "AH", "2C", "3C", "4C", "5C", "6C"},
        {"JJ", "9S", "8S", "7S", "6S", "5S", "4S", "3S", "2S", "JJ"}
    };
}

void DefaultBoardLayoutStrategy::initializeBoard(vector<vector<BoardCell>>& board) {
    for (int r = 0; r < BOARD_ROW_LENGTH; ++r) {
        vector<BoardCell> rowCells;
        for (int c = 0; c < BOARD_COL_LENGTH; ++c) {
            rowCells.push_back(BoardCell(r, c, Card(boardNotation[r][c]), 
                (boardNotation[r][c] == "JJ")));
        }
        board.push_back(rowCells);
    }
}
