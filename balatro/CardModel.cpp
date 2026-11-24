#include "CardModel.h"
#include <algorithm>
#include <functional>

using namespace std;

string Card::toString() const {
    static const char* suits = "SHDC";
    string r;
    switch (rank) {
        case 14: r = "A"; break;
        case 13: r = "K"; break;
        case 12: r = "Q"; break;
        case 11: r = "J"; break;
        default: r = to_string(rank);
    }
    return r + suits[suit];
}

string Hand::toString() const {
    string s;
    for (size_t i = 0; i < cards.size(); i++)
        s += "(" + to_string(i) + ")" + cards[i].toString() + " ";
    return s;
}

bool ChosenHand::selectFromHand(const Hand& hand, int index) {
    if (isFull()) return false;
    if (index < 0 || index >= static_cast<int>(hand.cards.size())) return false;
    if (find(pickedIndexes.begin(), pickedIndexes.end(), index) != pickedIndexes.end())
        return false;

    cards.push_back(hand.cards[index]);
    pickedIndexes.push_back(index);
    return true;
}

bool ChosenHand::addCard(const Card& card) {
    if (isFull()) return false;
    cards.push_back(card);
    pickedIndexes.push_back(-1);
    return true;
}

bool ChosenHand::removeCard(size_t pos) {
    if (pos >= cards.size()) return false;
    cards.erase(cards.begin() + pos);
    pickedIndexes.erase(pickedIndexes.begin() + pos);
    return true;
}

void ChosenHand::clear() {
    cards.clear();
    pickedIndexes.clear();
}

const vector<Card>& ChosenHand::getCards() const { return cards; }
size_t ChosenHand::size() const { return cards.size(); }
bool ChosenHand::isEmpty() const { return cards.empty(); }
bool ChosenHand::isFull() const { return cards.size() >= MAX_CARDS; }

string ChosenHand::toString() const {
    string s;
    for (const Card& c : cards) s += c.toString() + " ";
    return s;
}

vector<int> ChosenHand::rankCounts() const {
    int count[15] = {0};
    for (const Card& c : cards) count[c.rank]++;

    vector<int> result;
    for (int i = 2; i <= 14; i++)
        if (count[i] > 0) result.push_back(count[i]);

    sort(result.begin(), result.end(), greater<int>());
    return result;
}

int ChosenHand::maxSameRank() const {
    vector<int> c = rankCounts();
    return c.empty() ? 0 : c[0];
}

bool ChosenHand::isFlush() const {
    if (cards.size() != MAX_CARDS) return false;
    for (const Card& c : cards)
        if (c.suit != cards[0].suit) return false;
    return true;
}

bool ChosenHand::isStraight() const {
    if (cards.size() != MAX_CARDS) return false;

    vector<int> r;
    for (const Card& c : cards) r.push_back(c.rank);
    sort(r.begin(), r.end());

    for (size_t i = 1; i < r.size(); i++)
        if (r[i] == r[i - 1]) return false;

    if (r[4] - r[0] == 4) return true;
    return r == vector<int>({2, 3, 4, 5, 14});
}

bool ChosenHand::isRoyal() const {
    if (!isStraight()) return false;
    int minRank = 14;
    for (const Card& c : cards) minRank = min(minRank, c.rank);
    return minRank == 10;
}

string rankName(HandRank rank) {
    switch (rank) {
        case HandRank::HighCard:      return "High Card";
        case HandRank::Pair:          return "Pair";
        case HandRank::TwoPair:       return "Two Pair";
        case HandRank::ThreeOfAKind:  return "Three of a Kind";
        case HandRank::Straight:      return "Straight";
        case HandRank::Flush:         return "Flush";
        case HandRank::FullHouse:     return "Full House";
        case HandRank::FourOfAKind:   return "Four of a Kind";
        case HandRank::StraightFlush: return "Straight Flush";
        case HandRank::RoyalFlush:    return "Royal Flush";
        case HandRank::FiveOfAKind:   return "Five of a Kind";
        case HandRank::FlushHouse:    return "Flush House";
        case HandRank::FlushFive:     return "Flush Five";
        default:                      return "Unknown";
    }
}