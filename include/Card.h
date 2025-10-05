#ifndef CARD_H
#define CARD_H

#include "common.h"
#include <string>
#include <stdexcept>

class Card {
    Rank rank;
    Suit suit;
    CardType type;
public:
    Card(Rank r, Suit s, CardType type = NORMAL);
    Card(std::string notation, CardType type = NORMAL);
    std::pair<Rank, Suit> getCardIdentity() const;
    std::string toNotation() const;
    CardType getType() const;
};

#endif // CARD_H
