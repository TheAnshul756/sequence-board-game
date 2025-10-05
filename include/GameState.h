#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"
#include "Card.h"
#include <vector>

class GameState {
    Board board;
    friend class GameManager;
    std::vector<std::vector<Card>> playerCards;
    std::vector<Card> discardPile;
    std::vector<Card> remainingCards;
    ChipType currentTurn;
public:
    GameState(Board b, std::vector<std::vector<Card>> pCards, std::vector<Card> rCards);
    Board& getBoard();
    std::vector<Card>& getPlayerCards(int playerId);
    std::vector<Card> getRemainingCards() const;
    void drawCard(int playerId);
};

#endif // GAMESTATE_H
