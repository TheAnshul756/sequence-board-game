#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "IPlayer.h"
#include "PlayerView.h"
#include <iostream>

class HumanPlayer : public IPlayer {
    int playerId;
    PlayerView view;
    void printBoard();
    void printHand();
    Move getPositionInput(Card selectedCard);
public:
    HumanPlayer(int id, ChipType type, PlayerView& view);
    Move playTurn(const PlayerView& v) override;
    void notifyMove(const Move move, ChipType chip, const PlayerView& view) override;
};

#endif // HUMANPLAYER_H
