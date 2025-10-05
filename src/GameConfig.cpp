#include "GameConfig.h"

using namespace std;

GameConfig::GameConfig(int nPlayers, TeamMode m, IBoardLayoutStrategy* strategy, int nSequences)
    : numPlayers(nPlayers), mode(m), layoutStrategy(strategy), numWinningSequences(nSequences) {
        if (numPlayers == 2) {
            numCardsPerPlayer = 7;
        } else if (numPlayers == 3 || numPlayers == 4) {
            numCardsPerPlayer = 6;
        } else if (numPlayers == 6) {
            numCardsPerPlayer = 5;
        } else if (numPlayers == 8 || numPlayers == 9) {
            numCardsPerPlayer = 4;
        } else if (numPlayers == 10 || numPlayers == 12) {
            numCardsPerPlayer = 3;
        } else {
            throw invalid_argument("Unsupported number of players");
        } 
    }
