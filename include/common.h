#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <utility>

enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE, JOKER };
enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES, JOKER_SUIT }; 
enum ChipType { EMPTY, RED, GREEN, BLUE };
enum CardType {NORMAL, WILDPLACE, WILDREMOVE};
enum TeamMode { OneVsOne, TwoVsTwo, OneVsOneVsOne, TwoVsTwoVsTwo};

struct SequenceResult {
   int count;
   std::vector<std::vector<std::pair<int, int>>> positions;
};

class Card; // Forward declaration

extern const std::vector<std::string> ALL_CARDS;

#endif // COMMON_H
