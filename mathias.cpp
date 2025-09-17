#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

struct Card {
    std::string rank;
    std::string suit;
    int value;
};

std::vector<Card> createDeck() {
    std::vector<Card> deck;
    std::vector<std::string> suits = {"Hearts", "Diamonds", "Clubs", "Spades"};
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    std::vector<int> values   = {2,3,4,5,6,7,8,9,10,10,10,10,11};

    for (const auto& suit : suits) {
        for (size_t i = 0; i < ranks.size(); ++i) {
            deck.push_back({ranks[i], suit, values[i]});
        }
    }
    return deck;
}

void shuffleDeck(std::vector<Card>& deck) {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(deck.begin(), deck.end(), rng);
}

int handValue(const std::vector<Card>& hand) {
    int value = 0, aces = 0;
    for (const auto& card : hand) {
        value += card.value;
        if (card.rank == "A") ++aces;
    }
    while (value > 21 && aces > 0) {
        value -= 10;
        --aces;
    }
    return value;
}

void printHand(const std::vector<Card>& hand, bool hideFirst = false) {
    for (size_t i = 0; i < hand.size(); ++i) {
        if (i == 0 && hideFirst) {
            std::cout << "[Hidden], ";
        } else {
            std::cout << hand[i].rank << " of " << hand[i].suit;
            if (i != hand.size() - 1) std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::vector<Card> deck = createDeck();
    shuffleDeck(deck);

    std::vector<Card> playerHand, dealerHand;
    playerHand.push_back(deck.back()); deck.pop_back();
    dealerHand.push_back(deck.back()); deck.pop_back();
    playerHand.push_back(deck.back()); deck.pop_back();
    dealerHand.push_back(deck.back()); deck.pop_back();

    std::cout << "Dealer's hand: ";
    printHand(dealerHand, true);
    std::cout << "Your hand: ";
    printHand(playerHand);
    std::cout << "Your total: " << handValue(playerHand) << std::endl;

    // Player's turn
    while (true) {
        if (handValue(playerHand) == 21) {
            std::cout << "Blackjack! You win!\n";
            return 0;
        }
        if (handValue(playerHand) > 21) {
            std::cout << "Bust! You lose.\n";
            return 0;
        }
        std::cout << "Hit or stand? (h/s): ";
        char choice;
        std::cin >> choice;
        if (choice == 'h') {
            playerHand.push_back(deck.back()); deck.pop_back();
            std::cout << "Your hand: ";
            printHand(playerHand);
            std::cout << "Your total: " << handValue(playerHand) << std::endl;
        } else if (choice == 's') {
            break;
        }
    }

    // Dealer's turn
    std::cout << "Dealer's hand: ";
    printHand(dealerHand);
    std::cout << "Dealer's total: " << handValue(dealerHand) << std::endl;
    while (handValue(dealerHand) < 17) {
        dealerHand.push_back(deck.back()); deck.pop_back();
        std::cout << "Dealer hits: ";
        printHand(dealerHand);
        std::cout << "Dealer's total: " << handValue(dealerHand) << std::endl;
    }

    int playerTotal = handValue(playerHand);
    int dealerTotal = handValue(dealerHand);

    if (dealerTotal > 21) {
        std::cout << "Dealer busts! You win!\n";
    } else if (dealerTotal > playerTotal) {
        std::cout << "Dealer wins.\n";
    } else if (dealerTotal < playerTotal) {
        std::cout << "You win!\n";
    } else {
        std::cout << "Push (tie).\n";
    }
    return 0;
}