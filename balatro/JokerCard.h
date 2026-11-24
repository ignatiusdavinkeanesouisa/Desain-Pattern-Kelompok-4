#ifndef JOKER_CARD_H
#define JOKER_CARD_H

#include "CardModel.h"
#include <string>

// Paket informasi kejadian (Slide 14)
struct ScoreEvent {
    ChosenHand hand;
    HandRank pokerHand;
    int baseScore;
};

// Base Observer (Slide 17)
class JokerCard {
public:
    virtual ~JokerCard() = default;

    virtual std::string getName() const = 0;

    // Tahap 1: Menerima pemberitahuan saat skor dasar selesai dihitung
    virtual void onScoreCalculated(const ScoreEvent& eventData);

    // Tahap 2: Menerapkan efek modifikasi skor
    virtual int modifyScore(int currentScore, const ScoreEvent& eventData);
};

// Joker 1: Tambah +25 flat score di setiap ronde
class JesterChipJoker : public JokerCard {
public:
    std::string getName() const override;
    void onScoreCalculated(const ScoreEvent& eventData) override;
    int modifyScore(int currentScore, const ScoreEvent& eventData) override;
};

// Joker 2: Tambah +40 jika kombinasi Pair atau Two Pair
class DoubleTroubleJoker : public JokerCard {
public:
    std::string getName() const override;
    void onScoreCalculated(const ScoreEvent& eventData) override;
    int modifyScore(int currentScore, const ScoreEvent& eventData) override;
};

// Joker 3: Tambah +15 untuk setiap kartu As (rank 14) yang dimainkan
class AceHunterJoker : public JokerCard {
public:
    std::string getName() const override;
    void onScoreCalculated(const ScoreEvent& eventData) override;
    int modifyScore(int currentScore, const ScoreEvent& eventData) override;
};

// Joker 4: Kalikan skor x2 jika kombinasi Straight / Flush ke atas
class RoyalMultiplierJoker : public JokerCard {
public:
    std::string getName() const override;
    void onScoreCalculated(const ScoreEvent& eventData) override;
    int modifyScore(int currentScore, const ScoreEvent& eventData) override;
};

#endif
