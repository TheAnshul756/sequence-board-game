#ifndef DUMB1V1BOT_H
#define DUMB1V1BOT_H

#include "IPlayer.h"
#include "PlayerView.h"
#include <vector>

class Dumb1v1Bot : public IPlayer {
    int playerId;
    PlayerView view;
    void printBoard();
public:
    Dumb1v1Bot(int id, ChipType type, PlayerView& view);
    Move playTurn(const PlayerView& v) override;
    void notifyMove(const Move move, ChipType chip) override;
};

#endif // DUMB1V1BOT_H