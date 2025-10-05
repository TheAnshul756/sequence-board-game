#include "MediumLevel1v1Bot.h"
#include "Board.h"
#include "Card.h"
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

MediumLevel1v1Bot::MediumLevel1v1Bot(int id, ChipType type, PlayerView& v) : playerId(id), view(v) {
    chipType = type;
}

void MediumLevel1v1Bot::notifyMove(const Move move, ChipType chip, const PlayerView& v) {
    view = v;
    printBoard();
}

void MediumLevel1v1Bot::printBoard() {
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

// Helper function to evaluate position score
int evaluatePosition(const Board& board, int row, int col, ChipType playerChip) {
    int score = 0;
    const int directions[8][2] = {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}};
    
    for(auto [dr, dc] : directions) {
        int consecutive = 0;
        int empty = 0;
        
        // Check 4 positions in each direction
        for(int i = 1; i <= 4; i++) {
            int newRow = row + dr * i;
            int newCol = col + dc * i;
            
            if(newRow < 0 || newRow >= board.getNumRows() || 
               newCol < 0 || newCol >= board.getNumCols())
                break;
                
            BoardCell cell = board.getCell(newRow, newCol);
            if(cell.getChipType() == playerChip || cell.isCornerCell())
                consecutive++;
            else if(cell.isEmpty())
                empty++;
            else
                break;
        }
        
        // Scoring based on patterns
        if(consecutive == 4) score += 1000;  // Winning move
        else if(consecutive == 3 && empty == 1) score += 500;  // Near winning
        else if(consecutive == 2 && empty == 2) score += 100;  // Potential sequence
        else if(consecutive == 1 && empty == 3) score += 10;   // Starting sequence
    }
    
    // Bonus for center positions
    if((row >= 3 && row <= 6) && (col >= 3 && col <= 6))
        score += 50;
    
    return score;
}

Move MediumLevel1v1Bot::playTurn(const PlayerView& v) {
    view = v;
    vector<Card> hand = view.getPlayerCards();
    Board board = view.getBoard();
    
    // // Add small delay to simulate thinking
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Store all possible moves and their scores
    vector<pair<Move, int>> scoredMoves;
    
    // First, check for immediate winning moves or blocking opponent's wins
    for(const Card& card : hand) {
        for(int r = 0; r < board.getNumRows(); r++) {
            for(int c = 0; c < board.getNumCols(); c++) {
                BoardCell cell = board.getCell(r, c);
                
                if(card.getType() == WILDPLACE) {
                    if(cell.isEmpty() && !cell.isCornerCell()) {
                        int score = evaluatePosition(board, r, c, chipType);
                        scoredMoves.push_back({{r, c, card}, score});
                    }
                }
                else if(card.getType() == WILDREMOVE) {
                    if(!cell.isEmpty() && cell.getChipType() != chipType && !cell.isInSequence()) {
                        int score = evaluatePosition(board, r, c, chipType) * 2; // Prioritize blocking
                        scoredMoves.push_back({{r, c, card}, score});
                    }
                }
                else if(cell.isEmpty() && cell.getCard().toNotation() == card.toNotation()) {
                    int score = evaluatePosition(board, r, c, chipType);
                    scoredMoves.push_back({{r, c, card}, score});
                }
            }
        }
    }
    
    // If we found any moves, pick the one with highest score
    if(!scoredMoves.empty()) {
        auto bestMove = std::max_element(
            scoredMoves.begin(), 
            scoredMoves.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; }
        );
        
        cout << "MediumLevel1v1Bot playing card " << bestMove->first.card.toNotation() 
             << " at position (" << bestMove->first.row << "," << bestMove->first.col 
             << ") with score " << bestMove->second << endl;
             
        return bestMove->first;
    }
    
    // Fallback: if no good moves found, play first legal move
    for(const Card& card : hand) {
        for(int r = 0; r < board.getNumRows(); r++) {
            for(int c = 0; c < board.getNumCols(); c++) {
                BoardCell cell = board.getCell(r, c);
                if((card.getType() == WILDPLACE && cell.isEmpty() && !cell.isCornerCell()) ||
                   (card.getType() == WILDREMOVE && !cell.isEmpty() && cell.getChipType() != chipType && !cell.isInSequence()) ||
                   (cell.isEmpty() && cell.getCard().toNotation() == card.toNotation())) {
                    return Move{r, c, card};
                }
            }
        }
    }
    
    throw runtime_error("MediumLevel1v1Bot couldn't find any valid moves");
}