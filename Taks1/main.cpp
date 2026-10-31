#include <iostream>
#include <random>
#include <limits>
using namespace std;

// =====================================================
// BAGIAN 1: TASK 1 - CORE LOOP BALATRO
// =====================================================

// Data input untuk setiap ronde
struct TurnInput {
    int value;
};

// 1. Interface untuk membuat input
class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generate() = 0;
};

// Generator awal: selalu menghasilkan angka 3
class FixedInputGenerator : public IInputGenerator {
public:
    TurnInput generate() override {
        return {3};
    }
};

// Generator baru: menghasilkan angka acak 1 sampai 5
class RandomInputGenerator : public IInputGenerator {
private:
    mt19937 rng;
    uniform_int_distribution<int> dist;

public:
    RandomInputGenerator()
        : rng(random_device{}()), dist(1, 5) {}

    TurnInput generate() override {
        return {dist(rng)};
    }
};

// 2. Interface untuk menghitung skor
class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeBaseScore(const TurnInput& input) = 0;
};

// Skor dasar sama dengan nilai input
class SimpleScoringRule : public IScoringRule {
public:
    int computeBaseScore(const TurnInput& input) override {
        return input.value;
    }
};

// 3. Interface untuk menghitung reward
class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int computeReward(int baseScore, int roundNumber) = 0;
};

// Aturan reward awal
class SimpleRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore, int roundNumber) override {
        return baseScore;
    }
};

// Modifikasi reward: skor + 2
class BonusRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore, int roundNumber) override {
        return baseScore + 2;
    }
};

// 4. Sistem toko
class ShopSystem {
public:
    void showOffer() {
        cout << "[SHOP] offered: Bonus(+2) cost 2" << endl;
        cout << "[SHOP] skipped" << endl;
    }
};

// 5. Pengatur alur permainan
class RunSession {
private:
    IInputGenerator& inputGenerator;
    IScoringRule& scoringRule;
    IRewardRule& rewardRule;
    ShopSystem& shop;
    int money = 0;

public:
    RunSession(
        IInputGenerator& generator,
        IScoringRule& scorer,
        IRewardRule& reward,
        ShopSystem& shopSystem
    ) : inputGenerator(generator),
        scoringRule(scorer),
        rewardRule(reward),
        shop(shopSystem) {}

    void run() {
        cout << "=== RUN START ===" << endl;

        for (int round = 1; round <= 3; round++) {
            cout << "\nRound " << round << endl;

            // Phase 1: Generate input
            TurnInput input = inputGenerator.generate();

            cout << "[PLAY] input generated: "
                 << input.value << endl;

            // Phase 2: Compute base score
            int baseScore =
                scoringRule.computeBaseScore(input);

            cout << "[SCORE] base score: "
                 << baseScore << endl;

            // Phase 3: Compute reward
            int reward =
                rewardRule.computeReward(baseScore, round);

            cout << "[REWARD] gain: "
                 << reward << endl;

            // Phase 4: Update money
            money += reward;

            cout << "[MONEY] money: "
                 << money << endl;

            // Phase 5: Shop phase
            shop.showOffer();

            // Phase 6: Advance round
            cout << "[ROUND] advancing to next round"
                 << endl;
        }

        cout << "\n=== RUN END ===" << endl;
        cout << "Final money: " << money << endl;
    }
};


// =====================================================
// BAGIAN 2: POKER HAND CHECKER
// Chain of Responsibility Pattern
// =====================================================

// Jenis-jenis hand
enum class HandRank {
    Unknown = 0,
    HighCard,
    Pair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush,
    FiveOfAKind,
    FlushHouse,
    FlushFive
};

// Data Hand
struct Hand {
    HandRank rank = HandRank::Unknown;
};


// =====================================================
// BASE CLASS: POKER HAND CHECKER
// =====================================================

class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker = nullptr;

    // Memeriksa jenis hand atau meneruskan
    // pemeriksaan ke checker berikutnya
    HandRank checkOrForward(
        const Hand& hand,
        HandRank expectedRank,
        const string& handName
    ) {
        if (hand.rank == expectedRank) {
            cout << "[CHECKER] Hand type: "
                 << handName << endl;

            return expectedRank;
        }

        if (nextChecker != nullptr) {
            return nextChecker->check(hand);
        }

        return HandRank::Unknown;
    }

public:
    virtual ~PokerHandChecker() = default;

    virtual HandRank check(const Hand& hand) = 0;

    // Menghubungkan checker berikutnya
    void setNext(PokerHandChecker* next) {
        nextChecker = next;
    }
};


// =====================================================
// 13 HAND CHECKER
// =====================================================

class HighCardChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::HighCard, "High Card"
        );
    }
};

class PairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::Pair, "Pair"
        );
    }
};

class TwoPairChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::TwoPair, "Two Pair"
        );
    }
};

class ThreeOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::ThreeOfAKind, "Three of a Kind"
        );
    }
};

class StraightChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::Straight, "Straight"
        );
    }
};

class FlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::Flush, "Flush"
        );
    }
};

class FullHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::FullHouse, "Full House"
        );
    }
};

class FourOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::FourOfAKind, "Four of a Kind"
        );
    }
};

class StraightFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::StraightFlush, "Straight Flush"
        );
    }
};

class RoyalFlushChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::RoyalFlush, "Royal Flush"
        );
    }
};

class FiveOfAKindChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::FiveOfAKind, "Five of a Kind"
        );
    }
};

class FlushHouseChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::FlushHouse, "Flush House"
        );
    }
};

class FlushFiveChecker : public PokerHandChecker {
public:
    HandRank check(const Hand& hand) override {
        return checkOrForward(
            hand, HandRank::FlushFive, "Flush Five"
        );
    }
};


// =====================================================
// HAND GENERATOR
// =====================================================

class HandGenerator {
public:
    Hand generateHand() {
        return Hand{};
    }
};


// =====================================================
// HAND PLAYER
// =====================================================

class HandPlayer {
public:
    // Menentukan jenis hand untuk pengujian checker.
    // Tidak membutuhkan input keyboard.
    void setHandType(Hand& hand, HandRank rank) {
        hand.rank = rank;
    }
};


// =====================================================
// SCORING RULE
// Menghubungkan seluruh checker
// =====================================================

class ScoringRule {
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

public:
    ScoringRule() : firstChecker(&highCardChecker) {

        highCardChecker.setNext(&pairChecker);
        pairChecker.setNext(&twoPairChecker);
        twoPairChecker.setNext(&threeOfAKindChecker);
        threeOfAKindChecker.setNext(&straightChecker);
        straightChecker.setNext(&flushChecker);
        flushChecker.setNext(&fullHouseChecker);
        fullHouseChecker.setNext(&fourOfAKindChecker);
        fourOfAKindChecker.setNext(&straightFlushChecker);
        straightFlushChecker.setNext(&royalFlushChecker);
        royalFlushChecker.setNext(&fiveOfAKindChecker);
        fiveOfAKindChecker.setNext(&flushHouseChecker);
        flushHouseChecker.setNext(&flushFiveChecker);
    }

    HandRank checkHand(const Hand& hand) {
        return firstChecker->check(hand);
    }

    int convertRankToScore(HandRank rank) {
        return static_cast<int>(rank);
    }
};


// =====================================================
// BLIND RULE
// =====================================================

class BlindRule {
public:
    bool checkBlind(int score) {
        return score > 0;
    }
};


// =====================================================
// REWARD RULE
// =====================================================

class RewardRule {
public:
    int earnMoney(bool win, int score) {
        return win ? score : 0;
    }
};


// =====================================================
// GAME MANAGER
// =====================================================

class GameManager {
private:
    HandGenerator handGenerator;
    HandPlayer handPlayer;
    ScoringRule scoringRule;
    BlindRule blindRule;
    RewardRule rewardRule;

public:
    Hand generateHand() {
        return handGenerator.generateHand();
    }

    int scoreHand(const Hand& hand) {
        HandRank rank = scoringRule.checkHand(hand);

        return scoringRule.convertRankToScore(rank);
    }

    void runSession() {
        cout << "\n================================" << endl;
        cout << "      POKER HAND CHECKER" << endl;
        cout << "================================" << endl;

        // Menguji seluruh 13 checker secara otomatis.
        for (int i = 1; i <= 13; i++) {

            Hand hand = generateHand();

            // Menentukan jenis hand yang sedang diuji.
            handPlayer.setHandType(
                hand,
                static_cast<HandRank>(i)
            );

            // Memeriksa hand melalui checker chain.
            int score = scoreHand(hand);

            // Penggunaan BlindRule dan RewardRule
            bool win = blindRule.checkBlind(score);
            int reward = rewardRule.earnMoney(win, score);

            (void)reward;
        }
    }
};


// =====================================================
// MAIN PROGRAM
// =====================================================

int main() {

    // TASK 1
    RandomInputGenerator inputGenerator;

    SimpleScoringRule scoringRule;

    BonusRewardRule rewardRule;

    ShopSystem shop;

    RunSession session(
        inputGenerator,
        scoringRule,
        rewardRule,
        shop
    );

    // Menjalankan core loop 3 ronde.
    session.run();

    // TUGAS TAMBAHAN: 13 Poker Hand Checker.
    GameManager gameManager;

    gameManager.runSession();

    return 0;
}