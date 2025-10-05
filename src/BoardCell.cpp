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
    string chipSymbol;
    const string RESET = "\033[49m\033[0m";  // 49m resets background to default
    if (chipType == EMPTY) {
        chipSymbol = " ";
    } else if (chipType == RED) {
        chipSymbol = "\033[49m\033[31mX" + RESET;
    } else if (chipType == GREEN) {
        chipSymbol = "\033[49m\033[32mO" + RESET;
    } else {
        chipSymbol = "\033[49m\033[34m+" + RESET;
    }
    return card.toNotation() + chipSymbol + (inSequence ? "*" : " ");
}
