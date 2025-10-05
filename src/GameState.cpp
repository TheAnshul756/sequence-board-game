#include "GameState.h"
#include <stdexcept>

using namespace std;

GameState::GameState(Board b, vector<vector<Card>> pCards, vector<Card> rCards)
    : board(b), playerCards(pCards), remainingCards(rCards), currentTurn(RED) {}

Board& GameState::getBoard() { return board; }

vector<Card>& GameState::getPlayerCards(int playerId) { 
    if (playerId < 0 || playerId >= playerCards.size()) throw out_of_range("Invalid playerId");
    return playerCards[playerId];
}

vector<Card> GameState::getRemainingCards() const { return remainingCards; }

void GameState::drawCard(int playerId) {
    if (remainingCards.empty()) return;
    if (playerId < 0 || playerId >= playerCards.size()) return;
    playerCards[playerId].push_back(remainingCards.back());
    remainingCards.pop_back();
}
