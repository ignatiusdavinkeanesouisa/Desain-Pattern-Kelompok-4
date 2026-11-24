#include "JokerCard.h"
#include <iostream>

using namespace std;

void JokerCard::onScoreCalculated(const ScoreEvent& eventData) {}

int JokerCard::modifyScore(int currentScore, const ScoreEvent& eventData) {
    return currentScore;
}

// --- 1. JesterChipJoker ---
string JesterChipJoker::getName() const { return "Jester Chip Joker (+25)"; }

void JesterChipJoker::onScoreCalculated(const ScoreEvent& eventData) {
    cout << "  [Observer] " << getName()
         << " menerima kabar (Base Score: " << eventData.baseScore << ")" << endl;
}

int JesterChipJoker::modifyScore(int currentScore, const ScoreEvent& eventData) {
    int updated = currentScore + 25;
    cout << "  [Effect]   " << getName() << " aktif: "
         << currentScore << " -> " << updated << endl;
    return updated;
}

// --- 2. DoubleTroubleJoker ---
string DoubleTroubleJoker::getName() const { return "Double Trouble Joker (+40 Pair/TwoPair)"; }

void DoubleTroubleJoker::onScoreCalculated(const ScoreEvent& eventData) {
    cout << "  [Observer] " << getName()
         << " mengecek hand: " << rankName(eventData.pokerHand) << endl;
}

int DoubleTroubleJoker::modifyScore(int currentScore, const ScoreEvent& eventData) {
    if (eventData.pokerHand == HandRank::Pair ||
        eventData.pokerHand == HandRank::TwoPair) {
        int updated = currentScore + 40;
        cout << "  [Effect]   " << getName() << " aktif: "
             << currentScore << " -> " << updated << endl;
        return updated;
    }
    return currentScore;
}

// --- 3. AceHunterJoker ---
string AceHunterJoker::getName() const { return "Ace Hunter Joker (+15/Ace)"; }

void AceHunterJoker::onScoreCalculated(const ScoreEvent& eventData) {
    cout << "  [Observer] " << getName() << " memindai kartu As..." << endl;
}

int AceHunterJoker::modifyScore(int currentScore, const ScoreEvent& eventData) {
    int aceCount = 0;
    for (const Card& c : eventData.hand.getCards()) {
        if (c.rank == 14) aceCount++;
    }
    if (aceCount > 0) {
        int bonus = aceCount * 15;
        int updated = currentScore + bonus;
        cout << "  [Effect]   " << getName() << " aktif (" << aceCount << " As): "
             << currentScore << " -> " << updated << endl;
        return updated;
    }
    return currentScore;
}

// --- 4. RoyalMultiplierJoker ---
string RoyalMultiplierJoker::getName() const { return "Royal Multiplier Joker (x2 High Hand)"; }

void RoyalMultiplierJoker::onScoreCalculated(const ScoreEvent& eventData) {
    cout << "  [Observer] " << getName()
         << " mengecek kasta kombinasi: " << rankName(eventData.pokerHand) << endl;
}

int RoyalMultiplierJoker::modifyScore(int currentScore, const ScoreEvent& eventData) {
    if (static_cast<int>(eventData.pokerHand) >= static_cast<int>(HandRank::Straight)) {
        int updated = currentScore * 2;
        cout << "  [Effect]   " << getName() << " aktif (x2): "
             << currentScore << " -> " << updated << endl;
        return updated;
    }
    return currentScore;
}
