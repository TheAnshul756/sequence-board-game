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
    for (int r = 0; r < board.getNumRows(); ++r) {
        for (int c = 0; c < board.getNumCols(); ++c) {
            BoardCell cell = board.getCell(r, c);
            cout << cell.toNotation() << " ";
        }
        cout << endl;
    }
}

void HumanPlayer::printHand() {
    vector<Card> hand = view.getPlayerCards();
    cout << "Your hand: ";
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
    cout << "Player " << playerId << " placed/removed chip at (" 
         << move.row << ", " << move.col << ") using card " 
         << move.card.toNotation() << endl;
}
