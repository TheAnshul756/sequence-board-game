#include "BoardCell.h"

using namespace std;

BoardCell::BoardCell(int r, int c, Card c1, bool corner) 
    : row(r), col(c), card(c1), chipType(EMPTY), inSequence(false), isCorner(corner) {}

void BoardCell::setInSequence(bool val) { inSequence = val; }

void BoardCell::placeChip(ChipType type) { chipType = type; }

pair<int, int> BoardCell::getPosition() const { return {row, col}; }

Card BoardCell::getCard() const { return card; }

ChipType BoardCell::getChipType() const { return chipType; }

bool BoardCell::isInSequence() const { return inSequence; }

bool BoardCell::isCornerCell() const { return isCorner; }

bool BoardCell::isEmpty() const { return chipType == EMPTY; }

string BoardCell::toNotation() const {
    return card.toNotation() + 
           (chipType == EMPTY ? " " : (chipType == RED ? "X" : (chipType == GREEN ? "O" : "+"))) +
           (inSequence ? "*" : " ");
}
