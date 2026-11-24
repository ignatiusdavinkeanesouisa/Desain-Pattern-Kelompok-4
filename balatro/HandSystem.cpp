#include "HandSystem.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

HandGenerator::HandGenerator() : rng(random_device{}()) {}

Hand HandGenerator::generateHand(int handSize) {
    cout << "Generating hand..." << endl;

    vector<Card> deck;
    for (int s = 0; s < 4; s++)
        for (int r = 2; r <= 14; r++)
            deck.push_back({r, s});

    shuffle(deck.begin(), deck.end(), rng);

    Hand hand;
    int n = min(handSize, static_cast<int>(deck.size()));
    hand.cards.assign(deck.begin(), deck.begin() + n);
    return hand;
}

ChosenHand ConsoleHandPlayer::playHand(const Hand& hand) {
    cout << "[PLAY] your hand: " << hand.toString() << endl;
    cout << "Pilih sampai 5 index kartu, pisahkan spasi. Ketik -1 untuk selesai." << endl;

    ChosenHand chosen;
    while (!chosen.isFull()) {
        int idx;
        if (!(cin >> idx)) break;
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

    if (chosen.isEmpty()) chosen.selectFromHand(hand, 0);

    cout << "[PLAY] chosen hand: " << chosen.toString() << endl;
    return chosen;
}

RandomHandPlayer::RandomHandPlayer() : rng(random_device{}()) {}

ChosenHand RandomHandPlayer::playHand(const Hand& hand) {
    vector<int> idx(hand.cards.size());
    for (size_t i = 0; i < idx.size(); i++) idx[i] = static_cast<int>(i);
    shuffle(idx.begin(), idx.end(), rng);

    ChosenHand chosen;
    for (int i : idx)
        if (!chosen.selectFromHand(hand, i)) break;

    cout << "[PLAY] hand: " << hand.toString() << endl;
    cout << "[PLAY] chosen hand: " << chosen.toString() << endl;
    return chosen;
}