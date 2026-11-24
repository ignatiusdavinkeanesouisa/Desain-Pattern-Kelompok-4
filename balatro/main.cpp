#include "GameManager.h"
#include "JokerCard.h"
#include <iostream>
#include <vector>

using namespace std;

ChosenHand makeSampleHand(HandRank rank) {
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
        if (scorer.checkHand(chosen) == expected) passed++;
    }

    cout << "Hasil: " << passed << "/13 checker lulus" << endl;
}

int main() {
    HandGenerator handGenerator;
    ConsoleHandPlayer handPlayer; // Ganti ke RandomHandPlayer untuk simulasi otomatis
    PokerScoringRule scoringRule;
    BlindRule blindRule;
    BonusRewardRule rewardRule;
    ShopSystem shop;

    GameManager gameManager(handGenerator, handPlayer, scoringRule, blindRule, rewardRule, shop);

    // Membuat dan mendaftarkan 4 kartu Joker (Slide 15 & 18)
    JesterChipJoker jesterJoker;
    DoubleTroubleJoker doubleTroubleJoker;
    AceHunterJoker aceHunterJoker;
    RoyalMultiplierJoker royalMultJoker;

    gameManager.addJoker(&jesterJoker);
    gameManager.addJoker(&doubleTroubleJoker);
    gameManager.addJoker(&aceHunterJoker);
    gameManager.addJoker(&royalMultJoker);

    gameManager.run();
    runCheckerSelfTest();

    return 0;
}
