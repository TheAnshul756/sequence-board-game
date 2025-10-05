#include "DefaultBoardLayoutStrategy.h"

using namespace std;

DefaultBoardLayoutStrategy::DefaultBoardLayoutStrategy() {
    boardNotation = {
        {"JJ", "AC", "KC", "QC", "TC", "9C", "8C", "7C", "6C", "JJ"},
        {"AD", "7S", "8S", "9S", "TS", "QS", "KS", "AS", "5C", "2S"},
        {"KD", "6S", "TC", "9C", "8C", "7C", "6C", "2D", "4C", "3S"},
        {"QD", "5S", "QC", "8H", "7H", "6H", "5C", "3D", "3C", "4S"},
        {"TD", "4S", "KC", "9H", "2H", "5H", "4C", "4D", "2C", "5S"},
        {"9D", "3S", "AC", "TH", "3H", "4H", "3C", "5D", "AH", "6S"},
        {"8D", "2S", "AD", "QH", "KH", "AH", "2C", "6D", "KH", "7S"},
        {"7D", "2H", "KD", "QD", "TD", "9D", "8D", "7D", "QH", "8S"},
        {"6D", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "9S"},
        {"JJ", "5D", "4D", "3D", "2D", "AS", "KS", "QS", "TS", "JJ"},
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
