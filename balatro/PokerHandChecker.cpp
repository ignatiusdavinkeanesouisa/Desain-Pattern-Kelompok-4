#include "PokerHandChecker.h"
#include <iostream>

using namespace std;

void PokerHandChecker::setNext(PokerHandChecker* next) {
    nextChecker = next;
}

HandRank PokerHandChecker::check(const ChosenHand& hand) {
    if (matches(hand)) {
        cout << "[CHECKER] Hand type: " << rankName(rank()) << endl;
        return rank();
    }
    if (nextChecker != nullptr) return nextChecker->check(hand);
    return HandRank::Unknown;
}

bool HighCardChecker::matches(const ChosenHand& h) const { return !h.isEmpty(); }
HandRank HighCardChecker::rank() const { return HandRank::HighCard; }

bool PairChecker::matches(const ChosenHand& h) const { return h.maxSameRank() == 2; }
HandRank PairChecker::rank() const { return HandRank::Pair; }

bool TwoPairChecker::matches(const ChosenHand& h) const {
    auto c = h.rankCounts();
    return c.size() >= 2 && c[0] == 2 && c[1] == 2;
}
HandRank TwoPairChecker::rank() const { return HandRank::TwoPair; }

bool ThreeOfAKindChecker::matches(const ChosenHand& h) const { return h.maxSameRank() == 3; }
HandRank ThreeOfAKindChecker::rank() const { return HandRank::ThreeOfAKind; }

bool StraightChecker::matches(const ChosenHand& h) const { return h.isStraight(); }
HandRank StraightChecker::rank() const { return HandRank::Straight; }

bool FlushChecker::matches(const ChosenHand& h) const { return h.isFlush(); }
HandRank FlushChecker::rank() const { return HandRank::Flush; }

bool FullHouseChecker::matches(const ChosenHand& h) const {
    auto c = h.rankCounts();
    return c.size() == 2 && c[0] == 3 && c[1] == 2;
}
HandRank FullHouseChecker::rank() const { return HandRank::FullHouse; }

bool FourOfAKindChecker::matches(const ChosenHand& h) const { return h.maxSameRank() == 4; }
HandRank FourOfAKindChecker::rank() const { return HandRank::FourOfAKind; }

bool StraightFlushChecker::matches(const ChosenHand& h) const { return h.isStraight() && h.isFlush(); }
HandRank StraightFlushChecker::rank() const { return HandRank::StraightFlush; }

bool RoyalFlushChecker::matches(const ChosenHand& h) const { return h.isRoyal() && h.isFlush(); }
HandRank RoyalFlushChecker::rank() const { return HandRank::RoyalFlush; }

bool FiveOfAKindChecker::matches(const ChosenHand& h) const { return h.maxSameRank() == 5; }
HandRank FiveOfAKindChecker::rank() const { return HandRank::FiveOfAKind; }

bool FlushHouseChecker::matches(const ChosenHand& h) const {
    auto c = h.rankCounts();
    return c.size() == 2 && c[0] == 3 && c[1] == 2 && h.isFlush();
}
HandRank FlushHouseChecker::rank() const { return HandRank::FlushHouse; }

bool FlushFiveChecker::matches(const ChosenHand& h) const {
    return h.maxSameRank() == 5 && h.isFlush();
}
HandRank FlushFiveChecker::rank() const { return HandRank::FlushFive; }

PokerScoringRule::PokerScoringRule() : firstChecker(&flushFiveChecker) {
    flushFiveChecker.setNext(&flushHouseChecker);
    flushHouseChecker.setNext(&fiveOfAKindChecker);
    fiveOfAKindChecker.setNext(&royalFlushChecker);
    royalFlushChecker.setNext(&straightFlushChecker);
    straightFlushChecker.setNext(&fourOfAKindChecker);
    fourOfAKindChecker.setNext(&fullHouseChecker);
    fullHouseChecker.setNext(&flushChecker);
    flushChecker.setNext(&straightChecker);
    straightChecker.setNext(&threeOfAKindChecker);
    threeOfAKindChecker.setNext(&twoPairChecker);
    twoPairChecker.setNext(&pairChecker);
    pairChecker.setNext(&highCardChecker);
}

HandScoreTable& PokerScoringRule::getScoreTable() {
    return scoreTable;
}

HandRank PokerScoringRule::checkHand(const ChosenHand& chosen) {
    return firstChecker->check(chosen);
}

ScoreResult PokerScoringRule::getBaseScore(const ChosenHand& chosen) {
    HandRank detected = checkHand(chosen);
    int chips = scoreTable.getChips(detected);
    int mult = scoreTable.getMultiplier(detected);
    return {detected, chips, mult, chips * mult};
}
