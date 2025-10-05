#include "Dumb1v1Bot.h"
#include "Board.h"
#include "Card.h"
#include <vector>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

Dumb1v1Bot::Dumb1v1Bot(int id, ChipType type, PlayerView& v) : playerId(id), view(v) {
    chipType = type;
}

void Dumb1v1Bot::notifyMove(const Move move, ChipType chip) {
    // Bot does not need to do anything on notification
}

void Dumb1v1Bot::printBoard() {
    Board board = view.getBoard();
    cout << "\033[0m     ";  // Reset color
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << "  " << c << "  ";
    }
    cout << endl;
    for (int r = 0; r < board.getNumRows(); ++r) {
        // Alternate between dark and light background
        string bgColor = (r % 2 == 0) ? "\033[48;5;235m" : "\033[48;5;238m";
        cout << bgColor << "  " << r << "  \033[0m";  // Row number with background
        for (int c = 0; c < board.getNumCols(); ++c) {
            BoardCell cell = board.getCell(r, c);
            cout << bgColor;  // Set background color before each cell
            cout << cell.toNotation() << " \033[0m";  // Reset color after cell with space
        }
        cout << bgColor << "  " << r << "  \033[0m";  // Row number with background
        cout << endl;
    }
    cout << "     ";
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << "  " << c << "  ";
    }
    cout << endl;
}

Move Dumb1v1Bot::playTurn(const PlayerView& v) {
    view = v;
    printBoard();
    vector<Card> hand = view.getPlayerCards();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause for 5 seconds to simulate thinking
    do {
        int cardIndex = rand() % hand.size();
        Card selectedCard = hand[cardIndex];
        cout << "Dumb1v1Bot selected card: " << selectedCard.toNotation() << endl;
        for (int r = 0; r < view.getBoard().getNumRows(); ++r) {
            for (int c = 0; c < view.getBoard().getNumCols(); ++c) {
                BoardCell cell = view.getBoard().getCell(r, c);
                    if (selectedCard.getType() == WILDPLACE && cell.isEmpty() && !cell.isCornerCell()) {
                        cout << "Dumb1v1Bot placing at: (" << r << ", " << c << ")" << endl;
                        return Move{r, c, selectedCard};
                    } else if (selectedCard.getType() == WILDREMOVE && cell.getChipType() != chipType && !cell.isEmpty() && !cell.isInSequence()) {
                        cout << "Dumb1v1Bot removing at: (" << r << ", " << c << ")" << endl;
                        return Move{r, c, selectedCard};
                    } else if (cell.isEmpty() && cell.getCard().toNotation() == selectedCard.toNotation()) {
                        cout << "Dumb1v1Bot placing at: (" << r << ", " << c << ")" << endl;
                        return Move{r, c, selectedCard};
                    }
            }
        }
    } while(true);
    // Should never reach here
    throw std::runtime_error("Dumb1v1Bot failed to find a move");
}