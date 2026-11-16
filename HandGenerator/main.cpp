#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
using namespace std;

// =====================================================
// BAGIAN 1: MODEL KARTU, HAND, CHOSENHAND
// =====================================================

enum class HandRank {
    Unknown = 0,
    HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush,
    FullHouse, FourOfAKind, StraightFlush, RoyalFlush,
    FiveOfAKind, FlushHouse, FlushFive
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

// Hand = kartu yang dibagikan ke pemain (hasil HandGenerator)
struct Hand {
    vector<Card> cards;

    string toString() const {
        string s;
        for (size_t i = 0; i < cards.size(); i++)
            s += "(" + to_string(i) + ")" + cards[i].toString() + " ";
        return s;
    }
};

// -----------------------------------------------------
// TUGAS 1: ChosenHand
// Kartu yang DIPILIH pemain dari Hand (maksimal 5 kartu).
// -----------------------------------------------------
class ChosenHand {
public:
    static const size_t MAX_CARDS = 5;

private:
    vector<Card> cards;
    vector<int> pickedIndexes;  // index kartu di Hand (cegah pilih ganda)

public:
    // Pilih kartu dari Hand lewat index. false jika gagal.
    bool selectFromHand(const Hand& hand, int index) {
        if (isFull()) return false;
        if (index < 0 || index >= (int)hand.cards.size()) return false;
        if (find(pickedIndexes.begin(), pickedIndexes.end(), index)
            != pickedIndexes.end()) return false;  // sudah dipilih

        cards.push_back(hand.cards[index]);
        pickedIndexes.push_back(index);
        return true;
    }

    // Tambah kartu langsung (dipakai untuk self-test)
    bool addCard(const Card& card) {
        if (isFull()) return false;
        cards.push_back(card);
        pickedIndexes.push_back(-1);
        return true;
    }

    // Batalkan pilihan pada posisi ke-pos di ChosenHand
    bool removeCard(size_t pos) {
        if (pos >= cards.size()) return false;
        cards.erase(cards.begin() + pos);
        pickedIndexes.erase(pickedIndexes.begin() + pos);
        return true;
    }

    void clear() { cards.clear(); pickedIndexes.clear(); }

    const vector<Card>& getCards() const { return cards; }
    size_t size() const { return cards.size(); }
    bool isEmpty() const { return cards.empty(); }
    bool isFull() const { return cards.size() >= MAX_CARDS; }

    string toString() const {
        string s;
        for (const Card& c : cards) s += c.toString() + " ";
        return s;
    }

    // ---------- Fungsi bantu untuk checker ----------

    // Jumlah kemunculan tiap rank, urut dari terbesar. Full House -> {3,2}
    vector<int> rankCounts() const {
        int count[15] = {0};
        for (const Card& c : cards) count[c.rank]++;

        vector<int> result;
        for (int i = 2; i <= 14; i++)
            if (count[i] > 0) result.push_back(count[i]);

        sort(result.begin(), result.end(), greater<int>());
        return result;
    }

    // Jumlah kartu terbanyak dengan rank sama (0 jika kosong)
    int maxSameRank() const {
        vector<int> c = rankCounts();
        return c.empty() ? 0 : c[0];
    }

    bool isFlush() const {
        if (cards.size() != MAX_CARDS) return false;
        for (const Card& c : cards)
            if (c.suit != cards[0].suit) return false;
        return true;
    }

    bool isStraight() const {
        if (cards.size() != MAX_CARDS) return false;

        vector<int> r;
        for (const Card& c : cards) r.push_back(c.rank);
        sort(r.begin(), r.end());

        for (size_t i = 1; i < r.size(); i++)   // rank tidak boleh kembar
            if (r[i] == r[i - 1]) return false;

        if (r[4] - r[0] == 4) return true;       // straight biasa
        return r == vector<int>({2, 3, 4, 5, 14}); // wheel A-2-3-4-5
    }

    bool isRoyal() const {  // straight 10-J-Q-K-A
        if (!isStraight()) return false;
        int minRank = 14;
        for (const Card& c : cards) minRank = min(minRank, c.rank);
        return minRank == 10;
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
// BAGIAN 2: HAND GENERATOR (TUGAS 3: kartu random)
// =====================================================

class HandGenerator {
private:
    mt19937 rng;

public:
    HandGenerator() : rng(random_device{}()) {}

    // Buat deck 52 kartu, kocok, ambil handSize kartu teratas
    // (tidak ada kartu duplikat).
    Hand generateHand(int handSize = 8) {
        cout << "Generating hand..." << endl;

        vector<Card> deck;
        for (int s = 0; s < 4; s++)
            for (int r = 2; r <= 14; r++)
                deck.push_back({r, s});

        shuffle(deck.begin(), deck.end(), rng);

        Hand hand;
        int n = min(handSize, (int)deck.size());
        hand.cards.assign(deck.begin(), deck.begin() + n);
        return hand;
    }
};

// =====================================================
// BAGIAN 3: HAND PLAYER (pemain memilih ChosenHand)
// =====================================================

class IHandPlayer {
public:
    virtual ~IHandPlayer() = default;
    virtual ChosenHand playHand(const Hand& hand) = 0;
};

// Pemain memilih kartu lewat input konsol
class ConsoleHandPlayer : public IHandPlayer {
public:
    ChosenHand playHand(const Hand& hand) override {
        cout << "[PLAY] your hand: " << hand.toString() << endl;
        cout << "Pilih sampai 5 index kartu, pisahkan spasi. "
                "Ketik -1 untuk selesai." << endl;

        ChosenHand chosen;
        while (!chosen.isFull()) {
            int idx;
            if (!(cin >> idx)) break;       // input habis / error
            if (idx < 0) {
                if (chosen.isEmpty()) {
                    cout << "Pilih minimal 1 kartu!" << endl;
                    continue;
                }
                break;
            }
            if (!chosen.selectFromHand(hand, idx))
                cout << "Index tidak valid / sudah dipilih!" << endl;
        }

        // Jika input berhenti sebelum ada pilihan, ambil kartu pertama
        if (chosen.isEmpty()) chosen.selectFromHand(hand, 0);

        cout << "[PLAY] chosen hand: " << chosen.toString() << endl;
        return chosen;
    }
};

// Pemain otomatis: memilih 5 kartu acak (untuk demo tanpa input)
class RandomHandPlayer : public IHandPlayer {
private:
    mt19937 rng{random_device{}()};
public:
    ChosenHand playHand(const Hand& hand) override {
        vector<int> idx(hand.cards.size());
        for (size_t i = 0; i < idx.size(); i++) idx[i] = (int)i;
        shuffle(idx.begin(), idx.end(), rng);

        ChosenHand chosen;
        for (int i : idx) if (!chosen.selectFromHand(hand, i)) break;

        cout << "[PLAY] hand: " << hand.toString() << endl;
        cout << "[PLAY] chosen hand: " << chosen.toString() << endl;
        return chosen;
    }
};

// =====================================================
// BAGIAN 4: POKER HAND CHECKER (TUGAS 2)
// Chain of Responsibility, memeriksa ChosenHand
// =====================================================

class PokerHandChecker {
protected:
    PokerHandChecker* nextChecker = nullptr;

    virtual bool matches(const ChosenHand& hand) const = 0;
    virtual HandRank rank() const = 0;

public:
    virtual ~PokerHandChecker() = default;

    void setNext(PokerHandChecker* next) { nextChecker = next; }

    HandRank check(const ChosenHand& hand) {
        if (matches(hand)) {
            cout << "[CHECKER] Hand type: " << rankName(rank()) << endl;
            return rank();
        }
        if (nextChecker != nullptr) return nextChecker->check(hand);
        return HandRank::Unknown;
    }
};

// 1. High Card (fallback terakhir; Unknown jika kosong)
class HighCardChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return !h.isEmpty(); }
    HandRank rank() const override { return HandRank::HighCard; }
};

// 2. Pair
class PairChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.maxSameRank() == 2; }
    HandRank rank() const override { return HandRank::Pair; }
};

// 3. Two Pair
class TwoPairChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override {
        auto c = h.rankCounts();
        return c.size() >= 2 && c[0] == 2 && c[1] == 2;
    }
    HandRank rank() const override { return HandRank::TwoPair; }
};

// 4. Three of a Kind
class ThreeOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.maxSameRank() == 3; }
    HandRank rank() const override { return HandRank::ThreeOfAKind; }
};

// 5. Straight
class StraightChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.isStraight(); }
    HandRank rank() const override { return HandRank::Straight; }
};

// 6. Flush
class FlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.isFlush(); }
    HandRank rank() const override { return HandRank::Flush; }
};

// 7. Full House
class FullHouseChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override {
        auto c = h.rankCounts();
        return c.size() == 2 && c[0] == 3 && c[1] == 2;
    }
    HandRank rank() const override { return HandRank::FullHouse; }
};

// 8. Four of a Kind
class FourOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.maxSameRank() == 4; }
    HandRank rank() const override { return HandRank::FourOfAKind; }
};

// 9. Straight Flush
class StraightFlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.isStraight() && h.isFlush(); }
    HandRank rank() const override { return HandRank::StraightFlush; }
};

// 10. Royal Flush
class RoyalFlushChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.isRoyal() && h.isFlush(); }
    HandRank rank() const override { return HandRank::RoyalFlush; }
};

// 11. Five of a Kind
class FiveOfAKindChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override { return h.maxSameRank() == 5; }
    HandRank rank() const override { return HandRank::FiveOfAKind; }
};

// 12. Flush House
class FlushHouseChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override {
        auto c = h.rankCounts();
        return c.size() == 2 && c[0] == 3 && c[1] == 2 && h.isFlush();
    }
    HandRank rank() const override { return HandRank::FlushHouse; }
};

// 13. Flush Five
class FlushFiveChecker : public PokerHandChecker {
protected:
    bool matches(const ChosenHand& h) const override {
        return h.maxSameRank() == 5 && h.isFlush();
    }
    HandRank rank() const override { return HandRank::FlushFive; }
};

// =====================================================
// BAGIAN 5: SCORING, REWARD, SHOP, RUN SESSION
// =====================================================

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int computeBaseScore(const ChosenHand& chosen) = 0;
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

public:
    PokerScoringRule() : firstChecker(&flushFiveChecker) {
        // Urutan dari hand TERTINGGI ke TERENDAH
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

    HandRank checkHand(const ChosenHand& chosen) {
        return firstChecker->check(chosen);
    }

    int computeBaseScore(const ChosenHand& chosen) override {
        return static_cast<int>(checkHand(chosen));
    }
};

class IRewardRule {
public:
    virtual ~IRewardRule() = default;
    virtual int computeReward(int baseScore, int roundNumber) = 0;
};

class SimpleRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore, int roundNumber) override {
        return baseScore;
    }
};

class BonusRewardRule : public IRewardRule {
public:
    int computeReward(int baseScore, int roundNumber) override {
        return baseScore + 2;
    }
};

class ShopSystem {
public:
    void showOffer() {
        cout << "[SHOP] offered: Bonus(+2) cost 2" << endl;
        cout << "[SHOP] skipped" << endl;
    }
};

// Alur: generate hand -> pilih ChosenHand -> checker -> skor -> reward
class RunSession {
private:
    HandGenerator& handGenerator;
    IHandPlayer& handPlayer;
    IScoringRule& scoringRule;
    IRewardRule& rewardRule;
    ShopSystem& shop;
    int money = 0;

public:
    RunSession(HandGenerator& generator, IHandPlayer& player,
               IScoringRule& scorer, IRewardRule& reward, ShopSystem& shopSystem)
        : handGenerator(generator), handPlayer(player),
          scoringRule(scorer), rewardRule(reward), shop(shopSystem) {}

    void run() {
        cout << "=== RUN START ===" << endl;

        for (int round = 1; round <= 3; round++) {
            cout << "\nRound " << round << endl;

            // Phase 1: generate hand acak
            Hand hand = handGenerator.generateHand();

            // Phase 2: pemain memilih kartu -> ChosenHand
            ChosenHand chosen = handPlayer.playHand(hand);

            // Phase 3: checker chain + base score
            int baseScore = scoringRule.computeBaseScore(chosen);
            cout << "[SCORE] base score: " << baseScore << endl;

            // Phase 4: reward
            int reward = rewardRule.computeReward(baseScore, round);
            cout << "[REWARD] gain: " << reward << endl;

            // Phase 5: update uang
            money += reward;
            cout << "[MONEY] money: " << money << endl;

            // Phase 6: shop
            shop.showOffer();
            cout << "[ROUND] advancing to next round" << endl;
        }

        cout << "\n=== RUN END ===" << endl;
        cout << "Final money: " << money << endl;
    }
};

// =====================================================
// BAGIAN 6: SELF-TEST 13 CHECKER
// =====================================================

ChosenHand makeSampleHand(HandRank rank) {
    // Card{rank, suit}  suit: 0=S 1=H 2=D 3=C
    vector<Card> c;
    switch (rank) {
        case HandRank::HighCard:      c = {{2,0},{5,1},{9,2},{11,3},{13,0}}; break;
        case HandRank::Pair:          c = {{4,0},{4,1},{8,2},{10,3},{13,0}}; break;
        case HandRank::TwoPair:       c = {{4,0},{4,1},{9,2},{9,3},{13,0}}; break;
        case HandRank::ThreeOfAKind:  c = {{7,0},{7,1},{7,2},{10,3},{13,0}}; break;
        case HandRank::Straight:      c = {{5,0},{6,1},{7,2},{8,3},{9,0}}; break;
        case HandRank::Flush:         c = {{2,1},{5,1},{9,1},{11,1},{13,1}}; break;
        case HandRank::FullHouse:     c = {{6,0},{6,1},{6,2},{12,3},{12,0}}; break;
        case HandRank::FourOfAKind:   c = {{9,0},{9,1},{9,2},{9,3},{3,0}}; break;
        case HandRank::StraightFlush: c = {{4,2},{5,2},{6,2},{7,2},{8,2}}; break;
        case HandRank::RoyalFlush:    c = {{10,0},{11,0},{12,0},{13,0},{14,0}}; break;
        case HandRank::FiveOfAKind:   c = {{8,0},{8,1},{8,2},{8,3},{8,0}}; break;
        case HandRank::FlushHouse:    c = {{5,3},{5,3},{5,3},{11,3},{11,3}}; break;
        case HandRank::FlushFive:     c = {{12,1},{12,1},{12,1},{12,1},{12,1}}; break;
        default: break;
    }
    ChosenHand chosen;
    for (const Card& card : c) chosen.addCard(card);
    return chosen;
}

void runCheckerSelfTest() {
    cout << "\n================================" << endl;
    cout << "   SELF-TEST 13 POKER CHECKER" << endl;
    cout << "================================" << endl;

    PokerScoringRule scorer;
    int passed = 0;

    for (int i = 1; i <= 13; i++) {
        HandRank expected = static_cast<HandRank>(i);
        ChosenHand chosen = makeSampleHand(expected);

        cout << "\nTest " << i << " (" << rankName(expected) << ")" << endl;
        cout << "Hand: " << chosen.toString() << endl;

        bool ok = (scorer.checkHand(chosen) == expected);
        cout << (ok ? "[PASS]" : "[FAIL]") << endl;
        if (ok) passed++;
    }

    // Uji ChosenHand yang kurang dari 5 kartu
    cout << "\nTest tambahan: pilihan < 5 kartu" << endl;
    ChosenHand two;  two.addCard({7,0}); two.addCard({7,1});
    bool ok1 = scorer.checkHand(two) == HandRank::Pair;
    ChosenHand one;  one.addCard({13,2});
    bool ok2 = scorer.checkHand(one) == HandRank::HighCard;
    ChosenHand none;
    bool ok3 = scorer.checkHand(none) == HandRank::Unknown;
    cout << ((ok1 && ok2 && ok3) ? "[PASS]" : "[FAIL]") << endl;

    cout << "\nHasil: " << passed << "/13 checker lulus" << endl;
}

// =====================================================
// MAIN
// =====================================================

int main() {
    HandGenerator handGenerator;
    ConsoleHandPlayer handPlayer;      // ganti ke RandomHandPlayer untuk demo otomatis
    PokerScoringRule scoringRule;
    BonusRewardRule rewardRule;
    ShopSystem shop;

    RunSession session(handGenerator, handPlayer, scoringRule, rewardRule, shop);
    session.run();

    runCheckerSelfTest();
    return 0;
}
