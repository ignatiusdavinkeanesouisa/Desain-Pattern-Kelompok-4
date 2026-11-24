#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include <string>
#include <vector>

enum class HandRank {
    Unknown = 0,
    HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush,
    FullHouse, FourOfAKind, StraightFlush, RoyalFlush,
    FiveOfAKind, FlushHouse, FlushFive
};

struct Card {
    int rank;  // 2..14 (11=J, 12=Q, 13=K, 14=A)
    int suit;  // 0=Spades, 1=Hearts, 2=Diamonds, 3=Clubs

    std::string toString() const;
};

struct Hand {
    std::vector<Card> cards;

    std::string toString() const;
};

class ChosenHand {
public:
    static const size_t MAX_CARDS = 5;

private:
    std::vector<Card> cards;
    std::vector<int> pickedIndexes;

public:
    bool selectFromHand(const Hand& hand, int index);
    bool addCard(const Card& card);
    bool removeCard(size_t pos);
    void clear();

    const std::vector<Card>& getCards() const;
    size_t size() const;
    bool isEmpty() const;
    bool isFull() const;
    std::string toString() const;

    // Fungsi bantu untuk checker
    std::vector<int> rankCounts() const;
    int maxSameRank() const;
    bool isFlush() const;
    bool isStraight() const;
    bool isRoyal() const;
};

std::string rankName(HandRank rank);

#endif