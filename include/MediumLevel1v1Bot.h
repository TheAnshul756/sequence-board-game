#ifndef MEDIUMLEVEL1V1BOT_H
#define MEDIUMLEVEL1V1BOT_H

#include "IPlayer.h"
#include "PlayerView.h"
#include <vector>

class MediumLevel1v1Bot : public IPlayer {
    int playerId;
    PlayerView view;
    void printBoard();
public:
    MediumLevel1v1Bot(int id, ChipType type, PlayerView& view);
    Move playTurn(const PlayerView& v) override;
    void notifyMove(const Move move, ChipType chip, const PlayerView& view) override;
};

#endif // MEDIUMLEVEL1V1BOT_H