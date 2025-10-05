#ifndef IPLAYER_H
#define IPLAYER_H

#include "common.h"
#include "PlayerView.h"
#include "move.h"

class IPlayer {
protected:
    ChipType chipType;
public:
    virtual Move playTurn(const PlayerView& view) = 0;
    virtual void notifyMove(const Move move, ChipType chip, const PlayerView& view) = 0;
    virtual ChipType getChipType() const;
    virtual ~IPlayer() = default;
};

#endif // IPLAYER_H
