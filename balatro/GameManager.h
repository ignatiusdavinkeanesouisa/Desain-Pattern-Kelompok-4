#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "CardModel.h"
#include "HandSystem.h"
#include "PokerHandChecker.h"
#include "JokerCard.h"
#include <vector>

class BlindRule {
public:
    int getTargetScore(int roundNumber) const;
    bool checkBlind(int finalScore, int roundNumber) const;
};

class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int earnMoney(bool win, int finalScore, int roundNumber) = 0;
};

class BonusRewardRule : public IRewardRule {
public:
    int earnMoney(bool win, int finalScore, int roundNumber) override;
};

class ShopSystem {
public:
    void showOffer(HandScoreTable& scoreTable);
};

class GameManager {
private:
    HandGenerator& handGenerator;
    IHandPlayer& handPlayer;
    IScoringRule& scoringRule;
    BlindRule& blindRule;
    IRewardRule& rewardRule;
    ShopSystem& shop;

    std::vector<JokerCard*> jokers;
    int money = 0;

public:
    GameManager(HandGenerator& generator, IHandPlayer& player,
                IScoringRule& scorer, BlindRule& blind,
                IRewardRule& reward, ShopSystem& shopSystem);

    void addJoker(JokerCard* joker);
    void notifyAllObservers(const ScoreEvent& eventData);
    void run();
};

#endif
