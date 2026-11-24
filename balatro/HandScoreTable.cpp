#include "HandScoreTable.h"

HandScoreTable::HandScoreTable() {
    chipsTable[HandRank::Unknown]       = 0;   multiplierTable[HandRank::Unknown]       = 0;
    chipsTable[HandRank::HighCard]      = 5;   multiplierTable[HandRank::HighCard]      = 1;
    chipsTable[HandRank::Pair]          = 10;  multiplierTable[HandRank::Pair]          = 2;
    chipsTable[HandRank::TwoPair]       = 20;  multiplierTable[HandRank::TwoPair]       = 2;
    chipsTable[HandRank::ThreeOfAKind]  = 30;  multiplierTable[HandRank::ThreeOfAKind]  = 3;
    chipsTable[HandRank::Straight]      = 30;  multiplierTable[HandRank::Straight]      = 4;
    chipsTable[HandRank::Flush]         = 35;  multiplierTable[HandRank::Flush]         = 4;
    chipsTable[HandRank::FullHouse]     = 40;  multiplierTable[HandRank::FullHouse]     = 4;
    chipsTable[HandRank::FourOfAKind]   = 60;  multiplierTable[HandRank::FourOfAKind]   = 7;
    chipsTable[HandRank::StraightFlush] = 100; multiplierTable[HandRank::StraightFlush] = 8;
    chipsTable[HandRank::RoyalFlush]    = 100; multiplierTable[HandRank::RoyalFlush]    = 8;
    chipsTable[HandRank::FiveOfAKind]   = 120; multiplierTable[HandRank::FiveOfAKind]   = 12;
    chipsTable[HandRank::FlushHouse]    = 140; multiplierTable[HandRank::FlushHouse]    = 14;
    chipsTable[HandRank::FlushFive]     = 160; multiplierTable[HandRank::FlushFive]     = 16;
}

int HandScoreTable::getChips(HandRank hand) const {
    auto it = chipsTable.find(hand);
    return (it != chipsTable.end()) ? it->second : 0;
}

int HandScoreTable::getMultiplier(HandRank hand) const {
    auto it = multiplierTable.find(hand);
    return (it != multiplierTable.end()) ? it->second : 0;
}

void HandScoreTable::upgradeChips(HandRank hand, int amount) {
    chipsTable[hand] += amount;
}

void HandScoreTable::upgradeMultiplier(HandRank hand, int amount) {
    multiplierTable[hand] += amount;
}