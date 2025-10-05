#ifndef BOARD_H
#define BOARD_H

#include "BoardCell.h"
#include "IBoardLayoutStrategy.h"
#include <vector>
#include <stdexcept>

class Board {
    friend class GameManager;
    int numRows, numCols;
    std::vector<std::vector<BoardCell>> board;
    void placeChip(int row, int col, ChipType type);
    void removeChip(int row, int col);
    SequenceResult checkAndMarkSequences(ChipType type);
public:
    Board(IBoardLayoutStrategy* layoutStrategy);
    BoardCell getCell(int row, int col) const;
    bool canPlaceChip(int row, int col, ChipType type);
    bool canRemoveChip(int row, int col, ChipType type);
    int getNumRows() const;
    int getNumCols() const;
};

#endif // BOARD_H
