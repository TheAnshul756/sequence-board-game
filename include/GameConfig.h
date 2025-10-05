#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "common.h"
#include "IBoardLayoutStrategy.h"
#include <stdexcept>

struct GameConfig {
    int numPlayers;
    TeamMode mode;
    IBoardLayoutStrategy* layoutStrategy;
    int numWinningSequences;
    int numCardsPerPlayer;
    GameConfig(int nPlayers, TeamMode m, IBoardLayoutStrategy* strategy, int nSequences);
};

#endif // GAMECONFIG_H
