#include "GameManager.h"
#include "Board.h"
#include "Card.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

using namespace std;

extern const vector<string> ALL_CARDS;

GameManager::GameManager(GameConfig cfg, vector<IPlayer*> pls)
    : config(cfg), players(pls), currentPlayerIndex(0),
      gameState(Board(cfg.layoutStrategy), vector<vector<Card>>(cfg.numPlayers), vector<Card>()) {
    Board board(config.layoutStrategy);
    vector<vector<Card>> playerCards(cfg.numPlayers);
    vector<Card> remainingCards = initializeDeck();
    for (int i = 0; i < config.numPlayers; ++i) {
        playerCards[i] = drawInitialCards(remainingCards);
    }
    gameState = GameState(board, playerCards, remainingCards);
    setupTeams();
    currentPlayerIndex = 0;
}

vector<Card> GameManager::initializeDeck() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    vector<Card> deck;
    for(const string& notation : ALL_CARDS) {
        if (notation == "JC" || notation == "JD") {
            deck.push_back(Card(notation, WILDPLACE));
        } else if (notation == "JS" || notation == "JH") {
            deck.push_back(Card(notation, WILDREMOVE));
        } else {
            deck.push_back(Card(notation));
        }
    }
    // copy the same deck twice to make 104 cards
    vector<Card> fullDeck = deck;
    deck.insert(deck.end(), fullDeck.begin(), fullDeck.end());
    shuffle(deck.begin(), deck.end(), rng);
    return deck;
}

vector<Card> GameManager::drawInitialCards(vector<Card>& deck) {
    if (deck.size() < config.numCardsPerPlayer) 
        throw runtime_error("Not enough cards in the deck");
    vector<Card> hand(deck.end() - config.numCardsPerPlayer, deck.end());
    deck.erase(deck.end() - config.numCardsPerPlayer, deck.end());
    return hand;
}

void GameManager::setupTeams() {
    for (int i = 0; i < players.size(); ++i) {
        team[players[i]->getChipType()].push_back(players[i]);
    }
}

bool GameManager::checkWinCondition() {
    for (const auto& [chip, members] : team) {
        SequenceResult result = gameState.getBoard().checkAndMarkSequences(chip);
        if (result.count >= config.numWinningSequences) {
            return true;
        }
    }
    return false;
}

bool GameManager::validateMove(int playerIndex, Move move) {
    for(auto handCard : gameState.getPlayerCards(playerIndex)) {
        if(handCard.toNotation() == move.card.toNotation()) {
            return true;
        }
    }
    if (move.row < 0 || move.row >= gameState.getBoard().getNumRows() || move.col < 0 || move.col >= gameState.getBoard().getNumCols()) {
        return false;
    }
    return false;
}

void GameManager::processMove(int playerIndex, Move move) {
    IPlayer& player = *players[playerIndex];
    ChipType chipType = player.getChipType();
    Board& board = gameState.getBoard();
    switch (move.card.getType()) {
    case CardType::NORMAL:
        if (!board.canPlaceChip(move.row, move.col, chipType)) {
            throw invalid_argument("Cannot place chip at the specified position");
        }
        board.placeChip(move.row, move.col, chipType);
        break;
    case CardType::WILDPLACE:
        if (!board.canPlaceChip(move.row, move.col, chipType)) {
            throw invalid_argument("Cannot place chip at the specified position");
        }
        board.placeChip(move.row, move.col, chipType);
        break;
    case CardType::WILDREMOVE:
        if (!board.canRemoveChip(move.row, move.col, chipType)) {
            throw invalid_argument("Cannot remove chip at the specified position");
        }
        board.removeChip(move.row, move.col);
        break;
    default:
        throw invalid_argument("Invalid card type");
}
    // Remove played card from player's hand
    vector<Card>& hand = gameState.getPlayerCards(playerIndex);
    auto it = find_if(hand.begin(), hand.end(), [&](const Card& c) {
        return c.toNotation() == move.card.toNotation();
    });
    if (it != hand.end()) {
        hand.erase(it);
    } else {
        throw invalid_argument("Played card not found in player's hand");
    }
    // Draw a new card
    gameState.drawCard(playerIndex);
}

string getChipString(ChipType type) {
    switch(type) {
        case RED: return "Red";
        case BLUE: return "Blue";
        case GREEN: return "Green";
        default: return "Unknown";
    }
}

void GameManager::startGame() {

    while (true) {
        IPlayer& currentPlayer = *players[currentPlayerIndex];
        PlayerView view(currentPlayerIndex, gameState);
        Move move = currentPlayer.playTurn(view);
        validateMove(currentPlayerIndex, move);
        processMove(currentPlayerIndex, move);
        bool isGameWon = checkWinCondition();
        // Notify all players about the move
        for (int ind = 0; ind < players.size(); ++ind) {
            IPlayer* player = players[ind];
            view = PlayerView(ind, gameState);
            player->notifyMove(move, currentPlayer.getChipType(), view);
        }
        if (isGameWon) {
            cout << "Game Over!" << endl;
            cout << "Players with " << getChipString(currentPlayer.getChipType()) << " chips win!" << endl;
            break;
        }
        currentPlayerIndex = (currentPlayerIndex + 1) % config.numPlayers;
        if(gameState.getRemainingCards().empty()) {
            cout << "No more cards left. Game ends in a draw!" << endl;
            break;
        }
    }
}
