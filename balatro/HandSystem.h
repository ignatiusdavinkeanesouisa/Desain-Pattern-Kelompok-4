#ifndef HAND_SYSTEM_H
#define HAND_SYSTEM_H

#include "CardModel.h"
#include <random>

class HandGenerator {
private:
    std::mt19937 rng;

public:
    HandGenerator();
    Hand generateHand(int handSize = 8);
};

class IHandPlayer {
public:
    virtual ~IHandPlayer() = default;
    virtual ChosenHand playHand(const Hand& hand) = 0;
};

class ConsoleHandPlayer : public IHandPlayer {
public:
    ChosenHand playHand(const Hand& hand) override;
};

class RandomHandPlayer : public IHandPlayer {
private:
    std::mt19937 rng;

public:
    RandomHandPlayer();
    ChosenHand playHand(const Hand& hand) override;
};

#endif
