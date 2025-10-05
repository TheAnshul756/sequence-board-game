#include "HumanPlayer.h"
#include "Board.h"
#include "Card.h"
#include <vector>
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer(int id, ChipType type, PlayerView& v) : playerId(id), view(v) {
    chipType = type;
}

void HumanPlayer::printBoard() {
    Board board = view.getBoard();
    cout << "\033[0m     ";  // Reset color
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << "  " << c << "  ";
    }
    cout << endl;
    for (int r = 0; r < board.getNumRows(); ++r) {
        // Alternate between dark and light background
        string bgColor = (r % 2 == 0) ? "\033[48;5;235m" : "\033[48;5;238m";
        cout << bgColor << "  " << r << "  ";  // Apply background color to row number
        for (int c = 0; c < board.getNumCols(); ++c) {
            BoardCell cell = board.getCell(r, c);
            cout << cell.toNotation() << " ";
        }
        cout << bgColor << "  " << r << "  \033[0m";  // Apply background color to row number and reset
        cout << endl;
    }
    cout << "     ";
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << "  " << c << "  ";
    }
    cout << endl;
}

void HumanPlayer::printHand() {
    vector<Card> hand = view.getPlayerCards();
    cout << playerId << " player's hand: ";
    for (int i = 0; i < hand.size(); ++i) {
        cout << i << ") " << hand[i].toNotation() << " ";
    }
    cout << endl;
}

Move HumanPlayer::playTurn(const PlayerView& v) {
    view = v;
    printBoard();
    printHand();
    int cardIndex;
    cout << "Select card index to play: ";
    cin >> cardIndex;
    vector<Card> hand = view.getPlayerCards();
    if (cardIndex < 0 || cardIndex >= hand.size()) {
        throw invalid_argument("Invalid card index");
    }
    Card selectedCard = hand[cardIndex];
    int row, col;
    cout << "Enter row and column to place/remove chip (0-9): ";
    cin >> row >> col;
    return Move{row, col, selectedCard};
}

void HumanPlayer::notifyMove(const Move move, ChipType chip) {
    cout << "Chip " << chip << " placed/removed at (" 
         << move.row << ", " << move.col << ") using card " 
         << move.card.toNotation() << endl;
}
