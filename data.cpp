#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <random>
using namespace std;

enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, JOKER };
enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES, JOKER_SUIT }; 
enum ChipType { EMPTY, RED, GREEN, BLUE };
enum CardType {NORMAL, WILDPLACE, WILDREMOVE};
enum TeamMode { OneVsOne, TwoVsTwo, OneVsOneVsOne, TwoVsTwoVsTwo};

const vector<string> ALL_CARDS = {
    "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "JH", "QH", "KH", "AH",
    "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD", "JD", "QD", "KD", "AD",
    "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC", "JC", "QC", "KC", "AC",
    "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS", "JS", "QS", "KS", "AS",
};

class Card {
    Rank rank;
    Suit suit;
    CardType type;
public:
    Card(Rank r, Suit s, CardType type = NORMAL) : rank(r), suit(s), type(type) {}
    Card(string notation, CardType type = NORMAL) : type(type) {
        if (notation.length() < 2) throw invalid_argument("Invalid card string");
        if (notation == "JJ") { rank = JOKER; suit = JOKER_SUIT; return; }
        char r = notation[0];
        char s = notation[1];
        if (r >= '2' && r <= '9') rank = static_cast<Rank>(r - '2');
        else {
            switch (r) {
                case 'T': rank = TEN; break;
                case 'J': rank = JACK; break;
                case 'Q': rank = QUEEN; break;
                case 'K': rank = KING; break;
                case 'A': rank = ACE; break;
                default: throw invalid_argument("Invalid rank character");
            }
        }
        switch (s) {
            case 'H': suit = HEARTS; break;
            case 'D': suit = DIAMONDS; break;
            case 'C': suit = CLUBS; break;
            case 'S': suit = SPADES; break;
            default: throw invalid_argument("Invalid suit character");
        }
    }
    pair<Rank, Suit> getCardIdentity() const { return {rank, suit}; }
    string toNotation() const {
        string r;
        if (rank >= TWO && rank <= NINE) r = to_string(static_cast<int>(rank) + 2);
        else {
            switch (rank) {        
                case TEN: r = "T"; break;
                case JACK: r = "J"; break;
                case QUEEN: r = "Q"; break;
                case KING: r = "K"; break;
                case ACE: r = "A"; break;
                case JOKER: return "JJ";
            }
        }
        char s;
        switch (suit) {
            case HEARTS: s = 'H'; break;
            case DIAMONDS: s = 'D'; break;
            case CLUBS: s = 'C'; break;
            case SPADES: s = 'S'; break;
            case JOKER_SUIT: return "JJ";
        }
        return r + s;
    }
    CardType getType() const { return type; }
};

struct SequenceResult {
   int count;
   vector<vector<pair<int, int>>> positions;
};

class BoardCell {
    int row, col;
    Card card;
    ChipType chipType;
    bool inSequence;
    bool isCorner;
    void setInSequence(bool val) { inSequence = val; }
    void placeChip(ChipType type) { chipType = type; }
    friend class Board;
public:
    BoardCell(int r, int c, Card c1, bool corner = false) 
        : row(r), col(c), card(c1), chipType(EMPTY), inSequence(false), isCorner(corner) {}
    pair<int, int> getPosition() const { return {row, col}; }
    Card getCard() const { return card; }
    ChipType getChipType() const { return chipType; }
    bool isInSequence() const { return inSequence; }
    bool isCornerCell() const { return isCorner; }
    bool isEmpty() const { return chipType == EMPTY; }
    string toNotation() const {
        return card.toNotation() + 
               (chipType == EMPTY ? " " : (chipType == RED ? "X" : (chipType == GREEN ? "O" : "+"))) +
               (inSequence ? "*" : " ");
    }
};

class IBoardLayoutStrategy {
public:
    virtual void initializeBoard(vector<vector<BoardCell>>& board) = 0;
    virtual ~IBoardLayoutStrategy() = default;
};

class DefaultBoardLayoutStrategy : public IBoardLayoutStrategy {
const int BOARD_ROW_LENGTH = 10;
const int BOARD_COL_LENGTH = 10;
vector<vector<string>> boardNotation = {
    {"JJ", "AC", "KC", "QC", "TC", "9C", "8C", "7C", "6C", "JJ"},
    {"AD", "7S", "8S", "9S", "TS", "QS", "KS", "AS", "5C", "2S"},
    {"KD", "6S", "TC", "9C", "8C", "7C", "6C", "2D", "4C", "3S"},
    {"QD", "5S", "QC", "8H", "7H", "6H", "5C", "3D", "3C", "4S"},
    {"TD", "4S", "KC", "9H", "2H", "5H", "4C", "4D", "2C", "5S"},
    {"9D", "3S", "AC", "TH", "3H", "4H", "3C", "5D", "AH", "6S"},
    {"8D", "2S", "AD", "QH", "KH", "AH", "2C", "6D", "KH", "7S"},
    {"7D", "2H", "KD", "QD", "TD", "9D", "8D", "7D", "QH", "8S"},
    {"6D", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH", "9S"},
    {"JJ", "5D", "4D", "3D", "2D", "AS", "KS", "QS", "TS", "JJ"},
};
public:
    void initializeBoard(vector<vector<BoardCell>>& board) override {
        for (int r = 0; r < BOARD_ROW_LENGTH; ++r) {
            vector<BoardCell> rowCells;
            for (int c = 0; c < BOARD_COL_LENGTH; ++c) {
                rowCells.push_back(BoardCell(r, c, Card(boardNotation[r][c]), 
                    (boardNotation[r][c] == "JJ")));
            }
            board.push_back(rowCells);
        }
    }
};

class Board {
    friend class GameManager;
    int numRows, numCols;
    vector<vector<BoardCell>> board;
    void placeChip(int row, int col, ChipType type) {
        board[row][col].placeChip(type);
    }
    void removeChip(int row, int col) {
        board[row][col].placeChip(EMPTY);
    }
    SequenceResult checkAndMarkSequences(ChipType type) {
        // Unmark all sequences first
        for(auto& row : board) {
            for(auto& cell : row) {
                if(cell.isInSequence()) {
                    cell.setInSequence(false);
                }
            }
        }
        SequenceResult result = {0, {}};
        // Check horizontal sequences
        for(int i = 0; i < numCols; i++) {
            for(int j = 0; j < numRows - 4; j++) {
                bool allMatch = true;
                for(int k = 0; k < 5; k++) {
                    if(!(board[j + k][i].getChipType() == type || board[j + k][i].isCornerCell())) {
                        allMatch = false;
                        break;
                    }
                }
                if(allMatch) {
                    vector<pair<int, int>> positions;
                    for(int k = 0; k < 5; k++) {
                        board[j + k][i].setInSequence(true);
                        positions.push_back({j + k, i});
                    }
                    result.count++;
                    result.positions.push_back(positions);
                    j += 4;
                }
            }
        }
        // Check vertical sequences
        for(int i = 0; i < numRows; i++) {
            for(int j = 0; j < numCols - 4; j++) {
                bool allMatch = true;
                for(int k = 0; k < 5; k++) {
                    if(!(board[i][j + k].getChipType() == type || board[i][j + k].isCornerCell())) {
                        allMatch = false;
                        break; 
                    }
                }
                if(allMatch) {
                    vector<pair<int, int>> positions;
                    for(int k = 0; k < 5; k++) {
                        board[i][j + k].setInSequence(true);
                        positions.push_back({i, j + k});
                    }
                    result.count++;
                    result.positions.push_back(positions);
                    j += 4;
                }
            }
        }
        // check diagonal (top-left to bottom-right) sequences
        for(int i = 0; i < numRows - 4; i++) {
            for(int j = 0; j < numCols - 4; j++) {
                bool allMatch = true;
                for(int k = 0; k < 5; k++) {
                    if(!(board[i + k][j + k].getChipType() == type || board[i + k][j + k].isCornerCell())) {
                        allMatch = false;
                        break;
                    }
                }
                if (allMatch) {
                    vector<pair<int, int>> positions;
                    for(int k = 0; k < 5; k++) {
                        board[i + k][j + k].setInSequence(true);
                        positions.push_back({i + k, j + k});
                    }
                    result.count++;
                    result.positions.push_back(positions);
                    j += 4;
                }
            }
        }
        // check diagonal (top-right to bottom-left) sequences
        for(int i = 0; i < numRows - 4; i++) {
            for(int j = 4; j < numCols; j++) {
                bool allMatch = true;
                for(int k = 0; k < 5; k++) {
                    if(!(board[i + k][j - k].getChipType() == type || board[i + k][j - k].isCornerCell())) {
                        allMatch = false;
                        break; 
                    }
                }
                if (allMatch) {
                    vector<pair<int, int>> positions;
                    for(int k = 0; k < 5; k++) {
                        board[i + k][j - k].setInSequence(true);
                        positions.push_back({i + k, j - k});
                    }
                    result.count++;
                    result.positions.push_back(positions);
                    j -= 4;
                }
            }
        }
        return result;
    }
public:
    Board(IBoardLayoutStrategy* layoutStrategy) {
        layoutStrategy->initializeBoard(board);
        numRows = board.size();
        numCols = board[0].size();
    }
    BoardCell getCell(int row, int col) const {
        if (row < 0 || row >= numRows || col < 0 || col >= numCols) 
            throw out_of_range("Cell position out of range");
        return board[row][col];
    }
    bool canPlaceChip(int row, int col, ChipType type) {
        if (row < 0 || row >= numRows || col < 0 || col >= numCols) return false;
        BoardCell& cell = board[row][col];
        return cell.isEmpty() && !cell.isCornerCell();
    }
    bool canRemoveChip(int row, int col, ChipType type) {
        if (row < 0 || row >= numRows || col < 0 || col >= numCols) return false;
        BoardCell& cell = board[row][col];
        return !cell.isEmpty() && cell.getChipType() != type && !cell.isInSequence();
    }
    int getNumRows() const { return numRows; }
    int getNumCols() const { return numCols; }
};


class GameState {
    Board board;
    friend class GameManager;
    vector<vector<Card>> playerCards;
    vector<Card> discardPile;
    vector<Card> remainingCards;
    ChipType currentTurn;
public:
    GameState(Board b, vector<vector<Card>> pCards, vector<Card> rCards)
        : board(b), playerCards(pCards), remainingCards(rCards), currentTurn(RED) {}
    Board& getBoard() { return board; }
    vector<Card>& getPlayerCards(int playerId) { 
        if (playerId < 0 || playerId >= playerCards.size()) throw out_of_range("Invalid playerId");
        return playerCards[playerId];
    }
    vector<Card> getRemainingCards() const { return remainingCards; }
    void drawCard(int playerId) {
        if (remainingCards.empty()) return;
        if (playerId < 0 || playerId >= playerCards.size()) return;
        playerCards[playerId].push_back(remainingCards.back());
        remainingCards.pop_back();
    }
};

class PlayerView {
    int playerId;
    vector<Card> playerCards;
    Board board;
public:
    PlayerView(int id, GameState state) : playerId(id), board(state.getBoard()), playerCards(state.getPlayerCards(id)) {}
    friend class GameManager;
    BoardCell getCell(int row, int col) {
        BoardCell cell = board.getCell(row, col) ;
        return cell;
    }
    Board getBoard() {
        return board;
    }
    vector<Card> getPlayerCards() {
        return playerCards;
    }
};

struct GameConfig {
    int numPlayers;
    TeamMode mode;
    IBoardLayoutStrategy* layoutStrategy;
    int numWinningSequences;
    int numCardsPerPlayer;
    GameConfig(int nPlayers, TeamMode m, IBoardLayoutStrategy* strategy, int nSequences)
        : numPlayers(nPlayers), mode(m), layoutStrategy(strategy), numWinningSequences(nSequences) {
            if (numPlayers == 2) {
                numCardsPerPlayer = 7;
            } else if (numPlayers == 3 || numPlayers == 4) {
                numCardsPerPlayer = 6;
            } else if (numPlayers == 6) {
                numCardsPerPlayer = 5;
            } else if (numPlayers == 8 || numPlayers == 9) {
                numCardsPerPlayer = 4;
            } else if (numPlayers == 10 || numPlayers == 12) {
                numCardsPerPlayer = 3;
            } else {
                throw invalid_argument("Unsupported number of players");
            } 
        }
};

struct Move {
    int row, col;
    Card card;
};

class IPlayer {
protected:
    ChipType chipType;
public:
    virtual Move playTurn(const PlayerView& view) = 0;
    virtual void notifyMove(const Move move, ChipType chip) = 0;
    virtual ChipType getChipType() const { return chipType; }
    virtual ~IPlayer() = default;
};

class HumanPlayer : public IPlayer {
    int playerId;
    PlayerView view;
    void printBoard() {
        Board board = view.getBoard();
        for (int r = 0; r < board.getNumRows(); ++r) {
            for (int c = 0; c < board.getNumCols(); ++c) {
                BoardCell cell = board.getCell(r, c);
                cout << cell.toNotation() << " ";
            }
            cout << endl;
        }
    }
    void printHand() {
        vector<Card> hand = view.getPlayerCards();
        cout << "Your hand: ";
        for (int i = 0; i < hand.size(); ++i) {
            cout << i << ") " << hand[i].toNotation() << " ";
        }
        cout << endl;
    }
public:
    HumanPlayer(int id, ChipType type, PlayerView view) : playerId(id), view(view) {
        chipType = type;
    }
    Move playTurn(const PlayerView& v) override {
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
    void notifyMove(const Move move, ChipType chip) override {
        cout << "Player " << playerId << " placed/removed chip at (" 
             << move.row << ", " << move.col << ") using card " 
             << move.card.toNotation() << endl;
    }
};

class GameManager {
    GameState gameState;
    GameConfig config;
    vector<IPlayer*> players;
    map<ChipType, vector<IPlayer*>> team;
    int currentPlayerIndex;
public:
    GameManager(GameConfig cfg, vector<IPlayer*> pls)
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

    vector<Card> initializeDeck() {
        mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
        vector<Card> deck;
        for(const string& notation : ALL_CARDS) {
            if (notation == "JS" || notation == "JD") {
                deck.push_back(Card(notation, WILDPLACE));
            } else if (notation == "JC" || notation == "JH") {
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

    vector<Card> drawInitialCards(vector<Card>& deck) {
        if (deck.size() < config.numCardsPerPlayer) 
            throw runtime_error("Not enough cards in the deck");
        vector<Card> hand(deck.end() - config.numCardsPerPlayer, deck.end());
        deck.erase(deck.end() - config.numCardsPerPlayer, deck.end());
        return hand;
    }

    void setupTeams() {
        if (config.mode == OneVsOne) {
            team[RED] = {players[0]};
            team[GREEN] = {players[1]};
        } else if (config.mode == TwoVsTwo) {
            team[RED] = {players[0], players[2]};
            team[GREEN] = {players[1], players[3]};
        } else if (config.mode == OneVsOneVsOne) {
            team[RED] = {players[0]};
            team[GREEN] = {players[1]};
            team[BLUE] = {players[2]};
        } else if (config.mode == TwoVsTwoVsTwo) {
            team[RED] = {players[0], players[3]};
            team[GREEN] = {players[1], players[4]};
            team[BLUE] = {players[2], players[5]};
        } else {
            throw invalid_argument("Unsupported team mode");
        }
    }

    bool checkWinCondition() {
        for (const auto& [chip, members] : team) {
            SequenceResult result = gameState.getBoard().checkAndMarkSequences(chip);
            if (result.count >= config.numWinningSequences) {
                return true;
            }
        }
        return false;
    }

    bool validateMove(int playerIndex, Move move) {
        for(auto handCard : gameState.getPlayerCards(playerIndex)) {
            if(handCard.toNotation() == move.card.toNotation()) {
                return true;
            }
        }
        if (move.row < gameState.getBoard().numRows || move.row >= gameState.getBoard().numRows || move.col < gameState.getBoard().numCols || move.col >= gameState.getBoard().numCols) {
            return false;
        }
        return false;
    }

    void processMove(int playerIndex, Move move) {
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

    void startGame() {
        while (true) {
            IPlayer& currentPlayer = *players[currentPlayerIndex];
            PlayerView view(currentPlayerIndex, gameState);
            Move move = currentPlayer.playTurn(view);
            validateMove(currentPlayerIndex, move);
            processMove(currentPlayerIndex, move);
            for (auto& player : players) {
                player->notifyMove(move, currentPlayer.getChipType());
            }
            if (checkWinCondition()) {
                cout << "Game Over!" << endl;
                for (auto i : team[currentPlayer.getChipType()]) {
                    cout << "Player " << i->getChipType() << " wins!" << endl;
                }
            }
            currentPlayerIndex = (currentPlayerIndex + 1) % config.numPlayers;
            if(gameState.getRemainingCards().empty()) {
                cout << "No more cards left. Game ends in a draw!" << endl;
                break;
            }
        }
    }
};


int main() {
    DefaultBoardLayoutStrategy* layoutStrategy = new DefaultBoardLayoutStrategy();
    GameConfig config(2, OneVsOne, layoutStrategy, 2);

    vector<IPlayer*> players;
    for (int i = 0; i < 2; ++i) {
        ChipType chip = (i == 0) ? RED : GREEN;
        PlayerView view(i, GameState(Board(layoutStrategy), vector<vector<Card>>(2), vector<Card>()));
        players.push_back(new HumanPlayer(i, chip, view));
    }

    GameManager manager(config, players);
    manager.startGame();

    for (auto p : players) {
        delete p;
    }
    delete layoutStrategy;
    return 0;
}