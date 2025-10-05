#include "GameManager.h"
#include "HumanPlayer.h"
#include "Dumb1v1Bot.h"
#include "DefaultBoardLayoutStrategy.h"
#include "MediumLevel1v1Bot.h"
#include <iostream>
#include <vector>
using namespace std;

// int main() {
//     DefaultBoardLayoutStrategy* layoutStrategy = new DefaultBoardLayoutStrategy();
//     GameConfig config(2, OneVsOne, layoutStrategy, 2);

//     std::vector<IPlayer*> players;
//     Board board(layoutStrategy);
//     std::vector<std::vector<Card>> playerCards(2);
//     std::vector<Card> remainingCards; // This will be properly initialized in GameManager
//     GameState initialGameState(board, playerCards, remainingCards);

//     for (int i = 0; i < 2; ++i) {
//         ChipType chip = (i == 0) ? RED : GREEN;
//         PlayerView view(i, initialGameState);
//         players.push_back(new HumanPlayer(i, chip, view));
//     }

//     GameManager manager(config, players);
//     manager.startGame();

//     for (auto p : players) {
//         delete p;
//     }
//     delete layoutStrategy;
//     return 0;
// }

int game() {
    DefaultBoardLayoutStrategy* layoutStrategy = new DefaultBoardLayoutStrategy();
    GameConfig config(2, OneVsOne, layoutStrategy, 2);

    std::vector<IPlayer*> players;
    Board board(layoutStrategy);
    std::vector<std::vector<Card>> playerCards(2);
    std::vector<Card> remainingCards; // This will be properly initialized in GameManager
    GameState initialGameState(board, playerCards, remainingCards);

    // for (int i = 0; i < 2; ++i) {
    //     ChipType chip = (i == 0) ? RED : GREEN;
    //     PlayerView view(i, initialGameState);
    //     players.push_back(new HumanPlayer(i, chip, view));
    // }

    ChipType chip1 = RED;
    PlayerView view1(0, initialGameState);
    players.push_back(new Dumb1v1Bot(0, chip1, view1));
    ChipType chip2 = GREEN;
    PlayerView view2(1, initialGameState);
    players.push_back(new MediumLevel1v1Bot(1, chip2, view2));

    GameManager manager(config, players);
    manager.startGame();

    for (auto p : players) {
        delete p;
    }
    delete layoutStrategy;
    return manager.currentPlayerIndex;
}

int main() {
    vector<int> results(2, 0);
    for(int i = 0; i < 100; i++) {
        std::cout << "Starting game " << (i + 1) << std::endl;
        int out = game();
        results[out]++;
        std::cout << "Game " << (i + 1) << " ended." << std::endl;
    }
    std::cout << "Dumb1v1Bot wins: " << results[0] << ", MediumLevel1v1Bot wins: " << results[1] << std::endl;
    return 0;
}