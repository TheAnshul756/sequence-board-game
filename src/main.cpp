#include "GameManager.h"
#include "HumanPlayer.h"
#include "Dumb1v1Bot.h"
#include "DefaultBoardLayoutStrategy.h"
#include <iostream>
#include <vector>

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

int main() {
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
    players.push_back(new Dumb1v1Bot(1, chip2, view2));

    GameManager manager(config, players);
    manager.startGame();

    for (auto p : players) {
        delete p;
    }
    delete layoutStrategy;
    return 0;
}