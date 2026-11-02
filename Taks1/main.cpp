#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
using namespace std;

// =====================================================
// BAGIAN 1: MODEL KARTU & HAND
// =====================================================

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

struct Card {
    int rank;  // 2..14 (11=J, 12=Q, 13=K, 14=A)
    int suit;  // 0=Spades, 1=Hearts, 2=Diamonds, 3=Clubs

    string toString() const {
        static const char* suits = "SHDC";
        string r;
        switch (rank) {
            case 14: r = "A"; break;
            case 13: r = "K"; break;
            case 12: r = "Q"; break;
            case 11: r = "J"; break;
            default: r = to_string(rank);
        }
        return r + suits[suit];
    }
};

struct Hand {
    vector<Card> cards;

    // Jumlah kemunculan tiap rank, diurutkan dari terbesar.
    // Contoh Full House -> {3, 2}
    vector<int> rankCounts() const {
        int count[15] = {0};
        for (const Card& c : cards) count[c.rank]++;

        vector<int> result;
        for (int i = 2; i <= 14; i++)
            if (count[i] > 0) result.push_back(count[i]);

        sort(result.begin(), result.end(), greater<int>());
        return result;
    }

    bool isFlush() const {
        if (cards.size() != 5) return false;
        for (const Card& c : cards)
            if (c.suit != cards[0].suit) return false;
        return true;
    }

    bool isStraight() const {
        if (cards.size() != 5) return false;

        vector<int> r;
        for (const Card& c : cards) r.push_back(c.rank);
        sort(r.begin(), r.end());

        // Tidak boleh ada rank duplikat
        for (size_t i = 1; i < r.size(); i++)
            if (r[i] == r[i - 1]) return false;

        // Straight biasa
        if (r[4] - r[0] == 4) return true;

        // Wheel: A-2-3-4-5
        return r == vector<int>({2, 3, 4, 5, 14});
    }

    bool isRoyal() const {
        // Straight 10-J-Q-K-A
        if (!isStraight()) return false;
        int minRank = 14;
        for (const Card& c : cards) minRank = min(minRank, c.rank);
        return minRank == 10;
    }

    string toString() const {
        string s;
        for (const Card& c : cards) s += c.toString() + " ";
        return s;
    }
};

string rankName(HandRank rank) {
    switch (rank) {
        case HandRank::HighCard:      return "High Card";
        case HandRank::Pair:          return "Pair";
        case HandRank::TwoPair:       return "Two Pair";
        case HandRank::ThreeOfAKind:  return "Three of a Kind";
        case HandRank::Straight:      return "Straight";
        case HandRank::Flush:         return "Flush";
        case HandRank::FullHouse:     return "Full House";
        case HandRank::FourOfAKind:   return "Four of a Kind";
        case HandRank::StraightFlush: return "Straight Flush";
        case HandRank::RoyalFlush:    return "Royal Flush";
        case HandRank::FiveOfAKind:   return "Five of a Kind";
        case HandRank::FlushHouse:    return "Flush House";
        case HandRank::FlushFive:     return "Flush Five";
        default:                      return "Unknown";
    }
}

// =====================================================
// BAGIAN 2: POKER HAND CHECKER
// Chain of Responsibility Pattern
// =====================================================

class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker = nullptr;

    // Tiap checker menentukan sendiri apakah hand cocok
    virtual bool matches(const Hand& hand) const = 0;
    virtual HandRank rank() const = 0;

public:
    virtual ~PokerHandChecker() = default;

    void setNext(PokerHandChecker* next) { nextChecker = next; }

    // Jika cocok -> kembalikan rank, jika tidak -> teruskan ke checker berikutnya
    HandRank check(const Hand& hand) {
        if (matches(hand)) {
            cout << "[CHECKER] Hand type: " << rankName(rank()) << endl;
            return rank();
        }
        if (nextChecker != nullptr) return nextChecker->check(hand);
        return HandRank::Unknown;
    }
};

// ---------- 13 Checker ----------

// 1. High Card (selalu cocok, jadi fallback terakhir)
class HighCardChecker : public PokerHandChecker {
protected:
    bool matches(const Hand&) const override { return true; }
    HandRank rank() const override { return HandRank::HighCard; }
};

// 2. Pair
class PairChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.rankCounts()[0] == 2; }
    HandRank rank() const override { return HandRank::Pair; }
};

// 3. Two Pair
class TwoPairChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override {
        auto c = h.rankCounts();
        return c.size() >= 2 && c[0] == 2 && c[1] == 2;
    }
    HandRank rank() const override { return HandRank::TwoPair; }
};

// 4. Three of a Kind
class ThreeOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.rankCounts()[0] == 3; }
    HandRank rank() const override { return HandRank::ThreeOfAKind; }
};

// 5. Straight
class StraightChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.isStraight(); }
    HandRank rank() const override { return HandRank::Straight; }
};

// 6. Flush
class FlushChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.isFlush(); }
    HandRank rank() const override { return HandRank::Flush; }
};

// 7. Full House
class FullHouseChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override {
        auto c = h.rankCounts();
        return c.size() == 2 && c[0] == 3 && c[1] == 2;
    }
    HandRank rank() const override { return HandRank::FullHouse; }
};

// 8. Four of a Kind
class FourOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.rankCounts()[0] == 4; }
    HandRank rank() const override { return HandRank::FourOfAKind; }
};

// 9. Straight Flush
class StraightFlushChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.isStraight() && h.isFlush(); }
    HandRank rank() const override { return HandRank::StraightFlush; }
};

// 10. Royal Flush
class RoyalFlushChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.isRoyal() && h.isFlush(); }
    HandRank rank() const override { return HandRank::RoyalFlush; }
};

// 11. Five of a Kind (5 kartu rank sama)
class FiveOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override { return h.rankCounts()[0] == 5; }
    HandRank rank() const override { return HandRank::FiveOfAKind; }
};

// 12. Flush House (Full House + semua kartu satu suit)
class FlushHouseChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override {
        auto c = h.rankCounts();
        return c.size() == 2 && c[0] == 3 && c[1] == 2 && h.isFlush();
    }
    HandRank rank() const override { return HandRank::FlushHouse; }
};

// 13. Flush Five (Five of a Kind + satu suit)
class FlushFiveChecker : public PokerHandChecker {
protected:
    bool matches(const Hand& h) const override {
        return h.rankCounts()[0] == 5 && h.isFlush();
    }
    HandRank rank() const override { return HandRank::FlushFive; }
};

// =====================================================
// BAGIAN 3: TASK 1 - CORE LOOP BALATRO
// =====================================================

// Data input untuk setiap ronde
struct TurnInput {
    int value;   // nilai ringkas input
    Hand hand;   // 5 kartu yang dimainkan
};

// 1. Interface untuk membuat input
class IInputGenerator {
public:
    virtual ~IInputGenerator() = default;
    virtual TurnInput generate() = 0;
};

// Generator: membagikan 5 kartu acak dari deck 52 kartu
class PokerHandInputGenerator : public IInputGenerator {
private:
    mt19937 rng;

public:
    PokerHandInputGenerator() : rng(random_device{}()) {}

    TurnInput generate() override {
        vector<Card> deck;
        for (int s = 0; s < 4; s++)
            for (int r = 2; r <= 14; r++)
                deck.push_back({r, s});

        shuffle(deck.begin(), deck.end(), rng);

        TurnInput input;
        input.hand.cards.assign(deck.begin(), deck.begin() + 5);
        input.value = 0;
        return input;
    }
};

// 2. Interface untuk menghitung skor
class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeBaseScore(const TurnInput& input) = 0;
};

// Skor dasar = peringkat hand hasil checker chain
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

public:
    PokerScoringRule() : firstChecker(&flushFiveChecker) {
        // Urutan dari hand TERTINGGI ke TERENDAH supaya
        // yang terdeteksi selalu hand terbaik.
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

    HandRank checkHand(const Hand& hand) {
        return firstChecker->check(hand);
    }

    int computeBaseScore(const TurnInput& input) override {
        HandRank rank = checkHand(input.hand);
        return static_cast<int>(rank);
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

            // Phase 1: Generate input (5 kartu)
            TurnInput input = inputGenerator.generate();
            cout << "[PLAY] hand dealt: " << input.hand.toString() << endl;

            // Phase 2: Checker chain + base score
            // (di dalam computeBaseScore, 13 checker dijalankan)
            int baseScore = scoringRule.computeBaseScore(input);
            cout << "[SCORE] base score: " << baseScore << endl;

            // Phase 3: Compute reward
            int reward = rewardRule.computeReward(baseScore, round);
            cout << "[REWARD] gain: " << reward << endl;

            // Phase 4: Update money
            money += reward;
            cout << "[MONEY] money: " << money << endl;

            // Phase 5: Shop phase
            shop.showOffer();

            // Phase 6: Advance round
            cout << "[ROUND] advancing to next round" << endl;
        }

        cout << "\n=== RUN END ===" << endl;
        cout << "Final money: " << money << endl;
    }
};

// =====================================================
// BAGIAN 4: SELF-TEST 13 CHECKER
// Hand langka (Five of a Kind, dst.) hampir tidak pernah
// muncul dari deck biasa, jadi diuji dengan hand contoh.
// =====================================================

Hand makeSampleHand(HandRank rank) {
    // Card{rank, suit}  suit: 0=S 1=H 2=D 3=C
    switch (rank) {
        case HandRank::HighCard:
            return {{{2,0},{5,1},{9,2},{11,3},{13,0}}};
        case HandRank::Pair:
            return {{{4,0},{4,1},{8,2},{10,3},{13,0}}};
        case HandRank::TwoPair:
            return {{{4,0},{4,1},{9,2},{9,3},{13,0}}};
        case HandRank::ThreeOfAKind:
            return {{{7,0},{7,1},{7,2},{10,3},{13,0}}};
        case HandRank::Straight:
            return {{{5,0},{6,1},{7,2},{8,3},{9,0}}};
        case HandRank::Flush:
            return {{{2,1},{5,1},{9,1},{11,1},{13,1}}};
        case HandRank::FullHouse:
            return {{{6,0},{6,1},{6,2},{12,3},{12,0}}};
        case HandRank::FourOfAKind:
            return {{{9,0},{9,1},{9,2},{9,3},{3,0}}};
        case HandRank::StraightFlush:
            return {{{4,2},{5,2},{6,2},{7,2},{8,2}}};
        case HandRank::RoyalFlush:
            return {{{10,0},{11,0},{12,0},{13,0},{14,0}}};
        case HandRank::FiveOfAKind:
            return {{{8,0},{8,1},{8,2},{8,3},{8,0}}};
        case HandRank::FlushHouse:
            return {{{5,3},{5,3},{5,3},{11,3},{11,3}}};
        case HandRank::FlushFive:
            return {{{12,1},{12,1},{12,1},{12,1},{12,1}}};
        default:
            return {};
    }
}

void runCheckerSelfTest() {
    cout << "\n================================" << endl;
    cout << "   SELF-TEST 13 POKER CHECKER" << endl;
    cout << "================================" << endl;

    PokerScoringRule scorer;
    int passed = 0;

    for (int i = 1; i <= 13; i++) {
        HandRank expected = static_cast<HandRank>(i);
        Hand hand = makeSampleHand(expected);

        cout << "\nTest " << i << " (" << rankName(expected) << ")" << endl;
        cout << "Hand: " << hand.toString() << endl;

        HandRank result = scorer.checkHand(hand);

        bool ok = (result == expected);
        cout << (ok ? "[PASS]" : "[FAIL]") << endl;
        if (ok) passed++;
    }

    cout << "\nHasil: " << passed << "/13 checker lulus" << endl;
}

// =====================================================
// MAIN PROGRAM
// =====================================================

int main() {
    PokerHandInputGenerator inputGenerator;
    PokerScoringRule scoringRule;
    BonusRewardRule rewardRule;
    ShopSystem shop;

    RunSession session(inputGenerator, scoringRule, rewardRule, shop);

    // Core loop 3 ronde, tiap ronde menjalankan checker chain
    session.run();

    // Uji ke-13 checker dengan hand contoh
    runCheckerSelfTest();

    return 0;
}