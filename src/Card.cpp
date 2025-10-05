#include "Card.h"

using namespace std;

Card::Card(Rank r, Suit s, CardType type) : rank(r), suit(s), type(type) {}

Card::Card(string notation, CardType type) : type(type) {
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

pair<Rank, Suit> Card::getCardIdentity() const { return {rank, suit}; }

string Card::toNotation() const {
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

CardType Card::getType() const { return type; }
