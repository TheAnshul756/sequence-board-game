#ifndef PLAYERVIEW_H
#define PLAYERVIEW_H

#include "GameState.h"
#include "Board.h"
#include "Card.h"
#include <vector>

class PlayerView {
    int playerId;
    std::vector<Card> playerCards;
    Board board;
public:
    PlayerView(int id, GameState& state);
    friend class GameManager;
    BoardCell getCell(int row, int col);
    Board getBoard();
    std::vector<Card> getPlayerCards();
};

#endif // PLAYERVIEW_H
