#include "utils/CardUtils.h"

std::string CardUtils::suitToString(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS:    return "Clubs";
    case CST_DIAMONDS: return "Diamonds";
    case CST_HEARTS:   return "Hearts";
    case CST_SPADES:   return "Spades";
    default:           return "None";
    }
}

std::string CardUtils::faceToString(CardFaceType face)
{
    switch (face)
    {
    case CFT_ACE:   return "A";
    case CFT_TWO:   return "2";
    case CFT_THREE: return "3";
    case CFT_FOUR:  return "4";
    case CFT_FIVE:  return "5";
    case CFT_SIX:   return "6";
    case CFT_SEVEN: return "7";
    case CFT_EIGHT: return "8";
    case CFT_NINE:  return "9";
    case CFT_TEN:   return "10";
    case CFT_JACK:  return "J";
    case CFT_QUEEN: return "Q";
    case CFT_KING:  return "K";
    default:        return "None";
    }
}

bool CardUtils::isRedSuit(CardSuitType suit)
{
    return suit == CST_HEARTS || suit == CST_DIAMONDS;
}