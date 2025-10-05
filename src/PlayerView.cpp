#include "PlayerView.h"

using namespace std;

PlayerView::PlayerView(int id, GameState& state) : playerId(id), board(state.getBoard()), playerCards(state.getPlayerCards(id)) {}

BoardCell PlayerView::getCell(int row, int col) {
    BoardCell cell = board.getCell(row, col) ;
    return cell;
}

Board PlayerView::getBoard() {
    return board;
}

vector<Card> PlayerView::getPlayerCards() {
    return playerCards;
}
