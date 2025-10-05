#ifndef BOARDCELL_H
#define BOARDCELL_H

#include "common.h"
#include "Card.h"
#include <string>

class BoardCell {
    int row, col;
    Card card;
    ChipType chipType;
    bool inSequence;
    bool isCorner;
    void setInSequence(bool val);
    void placeChip(ChipType type);
    friend class Board;
public:
    BoardCell(int r, int c, Card c1, bool corner = false);
    std::pair<int, int> getPosition() const;
    Card getCard() const;
    ChipType getChipType() const;
    bool isInSequence() const;
    bool isCornerCell() const;
    bool isEmpty() const;
    std::string toNotation() const;
};

#endif // BOARDCELL_H
