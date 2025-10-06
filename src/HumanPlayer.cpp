#include "HumanPlayer.h"
#include "Board.h"
#include "Card.h"
#include <vector>
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer(int id, ChipType type, PlayerView& v) : playerId(id), view(v) {
    chipType = type;
}

bool isInHand(const Card& card, const vector<Card>& hand) {
    for (const auto& c : hand) {
        if (c.toNotation() == card.toNotation()) {
            return true;
        }
    }
    return false;
}

void HumanPlayer::printBoard() {
    string bgYellow = "\033[48;5;226m";  // Yellow background color
    Board board = view.getBoard();
    vector<Card> hand = view.getPlayerCards();
    cout << "\033[0m   ";  // Reset color
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << " " << c << " ";
    }
    cout << endl;
    for (int r = 0; r < board.getNumRows(); ++r) {
        // Alternate between dark and light background
        string bgColor = (r % 2 == 0) ? "\033[48;5;235m" : "\033[48;5;238m";
        cout << bgColor << " " << r << " \033[0m";  // Row number with background
        for (int c = 0; c < board.getNumCols(); ++c) {
            BoardCell cell = board.getCell(r, c);
            if(cell.isEmpty() && isInHand(cell.getCard(), hand)) {
                cout << bgYellow;  // Highlight available positions with yellow background
            } else {
                cout << bgColor;  // Set background color before each cell
            }
            cout << cell.toNotation() << "\033[0m";  // Reset color after cell with space
        }
        cout << bgColor << " " << r << "\033[0m";  // Row number with background
        cout << endl;
    }
    cout << "  ";
    for(int c = 0; c < board.getNumCols(); ++c) {
        cout << " " << c << " ";
    }
    cout << endl;
}

void HumanPlayer::printHand() {
    vector<Card> hand = view.getPlayerCards();
    cout << playerId << " player with chiptype " << (chipType == RED ? "X" : (chipType == GREEN ? "O" : "+")) << "'s hand: ";
    for (int i = 0; i < hand.size(); ++i) {
        cout << i << ") " << hand[i].toNotation() << " ";
    }
    cout << endl;
}

Move HumanPlayer::getPositionInput(Card selectedCard) {
    int row, col;
    if (selectedCard.getType() == WILDPLACE) {
        cout << "Enter row and column to place chip (0-9): ";
        cin >> row >> col;
    } else if (selectedCard.getType() == WILDREMOVE) {
        cout << "Enter row and column to remove chip (0-9): ";
        cin >> row >> col;
    } else {
        cout << "Enter row and column to place chip (0-9): ";
        cout << "Available positions for " << selectedCard.toNotation() << ":" << endl;
        vector<pair<int, int>> validPositions;
        Board board = view.getBoard();

        for (int r = 0; r < board.getNumRows(); ++r) {
            for (int c = 0; c < board.getNumCols(); ++c) {
                if (board.getCell(r, c).getCard().toNotation() == selectedCard.toNotation()) {
                    validPositions.push_back({r, c});
                    cout << validPositions.size()-1 << ") Position (" << r << "," << c << ")" << endl;
                }
            }
        }

        cout << "2) Discard the card" << endl;

        if (validPositions.empty()) {
            throw invalid_argument("No valid positions available for this card");
        }

        int choice;
        cout << "Enter position index: ";
        cin >> choice;

        if (choice < 0 || choice > validPositions.size()) {
            throw invalid_argument("Invalid position index");
        }
        if (choice == validPositions.size()) {
            return Move{-1, -1, selectedCard, true}; // Discard the card
        }

        row = validPositions[choice].first;
        col = validPositions[choice].second;
    }
    return Move{row, col, selectedCard};
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
    return getPositionInput(selectedCard);
}

void HumanPlayer::notifyMove(const Move move, ChipType chip, const PlayerView& view) {
    cout << "Chip " << chip << " placed/removed at (" 
         << move.row << ", " << move.col << ") using card " 
         << move.card.toNotation() << endl;
}
