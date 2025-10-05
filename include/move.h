#ifndef MOVE_H
#define MOVE_H

#include "Card.h"

struct Move {
    int row, col;
    Card card;
    bool discard; // true if the move is to discard the card
};

#endif // MOVE_H
