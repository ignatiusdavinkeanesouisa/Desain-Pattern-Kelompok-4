#ifndef POKER_HAND_CHECKER_H
#define POKER_HAND_CHECKER_H

#include "CardModel.h"
#include "HandScoreTable.h"

class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker = nullptr;

    virtual bool matches(const ChosenHand& hand) const = 0;
    virtual HandRank rank() const = 0;

public:
    virtual ~PokerHandChecker() = default;

    void setNext(PokerHandChecker* next);
    HandRank check(const ChosenHand& hand);
};

class HighCardChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class PairChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class TwoPairChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class ThreeOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class StraightChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FullHouseChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FourOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class StraightFlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class RoyalFlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FiveOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FlushHouseChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class FlushFiveChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override;
    HandRank rank() const override;
};

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual ScoreResult getBaseScore(const ChosenHand& chosen) = 0;
    virtual HandScoreTable& getScoreTable() = 0;
};

class PokerScoringRule : public IScoringRule {
private:
    HighCardChecker highCardChecker;
    PairChecker pairChecker;
    TwoPairChecker twoPairChecker;
    ThreeOfAKindChecker threeOfAKindChecker;
    StraightChecker straightChecker;
    FlushChecker flushChecker;
    FullHouseChecker fullHouseChecker;
    FourOfAKindChecker fourOfAKindChecker;
    StraightFlushChecker straightFlushChecker;
    RoyalFlushChecker royalFlushChecker;
    FiveOfAKindChecker fiveOfAKindChecker;
    FlushHouseChecker flushHouseChecker;
    FlushFiveChecker flushFiveChecker;

    PokerHandChecker* firstChecker;
    HandScoreTable scoreTable;

public:
    PokerScoringRule();

    HandScoreTable& getScoreTable() override;
    HandRank checkHand(const ChosenHand& chosen);
    ScoreResult getBaseScore(const ChosenHand& chosen) override;
};

#endif
