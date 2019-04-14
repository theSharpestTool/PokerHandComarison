// codewars.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <sstream>
#include <map>

using namespace std;

enum class HandValue
{
	Highcard, Pair, TwoPairs, Three, Straight, 
	Flush, FullHouse, Four, StraightFlush
};

struct PokerHand 
{
	struct Card
	{
		int value;
		char suit;
	};

	HandValue handValue;
	int highcardValue;
	std::vector<int> kikcers;

	PokerHand(const char* pokerhand) 
	{
		std::vector<Card> cards;
		std::stringstream sstream(pokerhand);
		std::string strCard;
		while (sstream >> strCard)
		{
			Card card{0, strCard[1]};
			switch (strCard[0])
			{
			case 'T': card.value = 10; break;
			case 'J': card.value = 11; break;
			case 'Q': card.value = 12; break;
			case 'K': card.value = 13; break;
			case 'A': card.value = 14; break;
			default: card.value = strCard[0] - '0'; break;
			}
			cards.push_back(card);
		}

		std::sort(cards.begin(), cards.end(), 
		[](Card& c1, Card& c2) -> bool 
		{ return c1.value < c2.value; });

		std::map<int, int> amountOfCards;
		for (auto& c : cards) amountOfCards[c.value]++;

		if (amountOfCards.size() == cards.size())
		{
			bool isStraight{ true }, isFlush{ true }, isLowAceStraight{ true };

			for (int i = 1; i < cards.size(); i++)
				if (cards[i].value != cards[i - 1].value + 1)
					isStraight = false;
			for (int i = 1; i < cards.size(); i++)
				if (cards[i].suit != cards[i - 1].suit)
					isFlush = false;
			for (int i = 0; i < cards.size() - 1; i++)
				if (cards[i].value != 2 + i) isLowAceStraight = false;
			if (cards.back().value != 14) isLowAceStraight = false;

			if (isLowAceStraight)
			{
				highcardValue = 5;
				isStraight = true;
			}
			else highcardValue = cards.back().value;

			if (isStraight) handValue = HandValue::Straight;
			if (isFlush) handValue = HandValue::Flush;
			if (isFlush && isStraight) handValue = HandValue::StraightFlush;
			if (!isFlush && !isStraight) handValue = HandValue::Highcard;

		}
		else {
			for (auto& card : amountOfCards)
			{
				switch (card.second)
				{
				case 2:
					if (handValue == HandValue::Pair)
						handValue = HandValue::TwoPairs;
					else
						handValue = HandValue::Pair;
					break;
				case 3:
					if (handValue == HandValue::Pair)
						handValue = HandValue::FullHouse;
					else
						handValue = HandValue::Three;
					break;
				case 4:
					handValue = HandValue::Four;
					break;
				}
				if (card.second > 1) highcardValue = card.first;
				else kikcers.push_back(card.first);
			}
		}

		if (handValue == HandValue::Highcard)
			for (int i = 0; i < cards.size() - 1; i++)
				kikcers.push_back(cards[i].value);

		std::reverse(kikcers.begin(), kikcers.end());
	}
};

enum class Result { Win, Loss, Tie };

Result compare(const PokerHand &player, const PokerHand &opponent) 
{
	if (player.handValue > opponent.handValue)
		return Result::Win;
	if (player.handValue < opponent.handValue)
		return Result::Loss;

	if(player.highcardValue > opponent.highcardValue)
		return Result::Win;
	if (player.highcardValue < opponent.highcardValue)
		return Result::Loss;

	for(int i = 0; i < player.kikcers.size(); i++)
		if(player.kikcers[i] > opponent.kikcers[i])
			return Result::Win;
		else if (player.kikcers[i] < opponent.kikcers[i])
			return Result::Loss;

	return Result::Tie;
}


int main()
{
	system("pause");
    return 0;
}