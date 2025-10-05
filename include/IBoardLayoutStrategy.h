#ifndef IBOARDLAYOUTSTRATEGY_H
#define IBOARDLAYOUTSTRATEGY_H

#include <vector>

class BoardCell; // Forward declaration

class IBoardLayoutStrategy {
public:
    virtual void initializeBoard(std::vector<std::vector<BoardCell>>& board) = 0;
    virtual ~IBoardLayoutStrategy() = default;
};

#endif // IBOARDLAYOUTSTRATEGY_H
