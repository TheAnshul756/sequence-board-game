#ifndef DEFAULTBOARDLAYOUTSTRATEGY_H
#define DEFAULTBOARDLAYOUTSTRATEGY_H

#include "IBoardLayoutStrategy.h"
#include "BoardCell.h"
#include <vector>
#include <string>

class DefaultBoardLayoutStrategy : public IBoardLayoutStrategy {
    const int BOARD_ROW_LENGTH = 10;
    const int BOARD_COL_LENGTH = 10;
    std::vector<std::vector<std::string>> boardNotation;
public:
    DefaultBoardLayoutStrategy();
    void initializeBoard(std::vector<std::vector<BoardCell>>& board) override;
};

#endif // DEFAULTBOARDLAYOUTSTRATEGY_H
