#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameState.h"
#include "GameConfig.h"
#include "IPlayer.h"
#include <vector>
#include <map>

class GameManager {
    GameState gameState;
    GameConfig config;
    std::vector<IPlayer*> players;
    std::map<ChipType, std::vector<IPlayer*>> team;
    int currentPlayerIndex;

    std::vector<Card> initializeDeck();
    std::vector<Card> drawInitialCards(std::vector<Card>& deck);
    void setupTeams();
    bool checkWinCondition();
    bool validateMove(int playerIndex, Move move);
    void processMove(int playerIndex, Move move);

public:
    GameManager(GameConfig cfg, std::vector<IPlayer*> pls);
    void startGame();
};

#endif // GAMEMANAGER_H
