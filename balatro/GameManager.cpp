#include "GameManager.h"
#include <iostream>

using namespace std;

int BlindRule::getTargetScore(int roundNumber) const {
    return roundNumber * 45; // Ronde 1: 45, Ronde 2: 90, Ronde 3: 135
}

bool BlindRule::checkBlind(int finalScore, int roundNumber) const {
    return finalScore >= getTargetScore(roundNumber);
}

int BonusRewardRule::earnMoney(bool win, int finalScore, int roundNumber) {
    if (!win) return 0;
    return 3 + roundNumber;
}

void ShopSystem::showOffer(HandScoreTable& scoreTable) {
    cout << "[SHOP] Upgrading Pair (+5 Chips, +1 Mult)..." << endl;
    scoreTable.upgradeChips(HandRank::Pair, 5);
    scoreTable.upgradeMultiplier(HandRank::Pair, 1);
}

GameManager::GameManager(HandGenerator& generator, IHandPlayer& player,
                         IScoringRule& scorer, BlindRule& blind,
                         IRewardRule& reward, ShopSystem& shopSystem)
    : handGenerator(generator), handPlayer(player),
      scoringRule(scorer), blindRule(blind),
      rewardRule(reward), shop(shopSystem) {}

void GameManager::addJoker(JokerCard* joker) {
    if (joker != nullptr) {
        jokers.push_back(joker);
    }
}

void GameManager::notifyAllObservers(const ScoreEvent& eventData) {
    cout << "[EVENT] Mengirim ScoreEvent ke " << jokers.size() << " Joker terdaftar..." << endl;
    for (JokerCard* joker : jokers) {
        joker->onScoreCalculated(eventData);
    }
}

void GameManager::run() {
    cout << "=== GAME START ===" << endl;

    for (int round = 1; round <= 3; round++) {
        int target = blindRule.getTargetScore(round);
        cout << "\n==========================================" << endl;
        cout << "ROUND " << round << " | Target Blind: " << target << endl;
        cout << "==========================================" << endl;

        // 1. Siapkan kartu
        Hand hand = handGenerator.generateHand();

        // 2. Pemain memilih kartu
        ChosenHand chosen = handPlayer.playHand(hand);

        // 3. Hitung skor dasar (Chips x Multiplier)
        ScoreResult result = scoringRule.getBaseScore(chosen);
        cout << "[SCORE] Base score: " << result.chips << " x " << result.multiplier
             << " = " << result.baseScore << endl;

        // 4. Siapkan informasi kejadian
        ScoreEvent eventData{chosen, result.pokerHand, result.baseScore};

        // 5. Kirim pemberitahuan ke semua Joker (Observer)
        notifyAllObservers(eventData);

        // 6. Terapkan efek modifikasi skor sesuai urutan pendaftaran Joker
        int finalScore = result.baseScore;
        for (JokerCard* joker : jokers) {
            finalScore = joker->modifyScore(finalScore, eventData);
        }
        cout << "[SCORE] Final score: " << finalScore << endl;

        // 7. Periksa kemenangan
        bool win = blindRule.checkBlind(finalScore, round);
        cout << "[BLIND] Hasil: " << (win ? "MENANG!" : "KALAH (skor di bawah target)") << endl;

        // 8. Hitung hadiah
        int reward = rewardRule.earnMoney(win, finalScore, round);
        money += reward;
        cout << "[REWARD] Gain: +" << reward << " | Total Money: " << money << endl;

        if (win) {
            shop.showOffer(scoringRule.getScoreTable());
        }
    }

    cout << "\n=== GAME END ===" << endl;
    cout << "Final money: " << money << endl;
}
