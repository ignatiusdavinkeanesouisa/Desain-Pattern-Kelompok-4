#ifndef HAND_SCORE_TABLE_H
#define HAND_SCORE_TABLE_H

#include "CardModel.h"
#include <map>

struct ScoreResult {
    HandRank pokerHand;
    int chips;
    int multiplier;
    int baseScore;
};

class HandScoreTable {
private:
    std::map<HandRank, int> chipsTable;
    std::map<HandRank, int> multiplierTable;

public:
    HandScoreTable();

    int getChips(HandRank hand) const;
    int getMultiplier(HandRank hand) const;

    void upgradeChips(HandRank hand, int amount);
    void upgradeMultiplier(HandRank hand, int amount);
};

#endif
