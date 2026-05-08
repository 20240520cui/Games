#include "configs/models/CardResConfig.h"

static const std::string kCardGeneralResPath = "res/card_general.png";

static const std::string kSuitsResPath = "res/suits/";

const std::string& CardResConfig::getCardGeneralResPath()
{
    return kCardGeneralResPath;
}

const std::string& getNumberResPathStatic(CardFaceType face, bool isBig, bool isBlack)
{
    static std::map<int, std::string> cache;
    int key = face * 100 + (isBig ? 10 : 0) + (isBlack ? 1 : 0);
    auto it = cache.find(key);
    if (it != cache.end()) return it->second;

    std::string name = CardResConfig::buildNumberResName(face, isBig, isBlack);
    cache[key] = "res/number/" + name + ".png";
    return cache[key];
}

const std::string& CardResConfig::getNumberResPath(CardFaceType face, bool isBig, bool isBlack)
{
    return getNumberResPathStatic(face, isBig, isBlack);
}

static const std::string kClubResPath = "res/suits/club.png";
static const std::string kDiamondResPath = "res/suits/diamond.png";
static const std::string kHeartResPath = "res/suits/heart.png";
static const std::string kSpadeResPath = "res/suits/spade.png";
static const std::string kEmptyResPath = "";

const std::string& CardResConfig::getSuitResPath(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS:    return kClubResPath;
    case CST_DIAMONDS: return kDiamondResPath;
    case CST_HEARTS:   return kHeartResPath;
    case CST_SPADES:   return kSpadeResPath;
    default:           return kEmptyResPath;
    }
}

std::string CardResConfig::buildNumberResName(CardFaceType face, bool isBig, bool isBlack)
{
    std::string sizePrefix = isBig ? "big_" : "small_";
    std::string colorPrefix = isBlack ? "black_" : "red_";
    return sizePrefix + colorPrefix + faceTypeToString(face);
}

std::string CardResConfig::buildSuitResName(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS:    return "club";
    case CST_DIAMONDS: return "diamond";
    case CST_HEARTS:   return "heart";
    case CST_SPADES:   return "spade";
    default:           return "";
    }
}

std::string CardResConfig::faceTypeToString(CardFaceType face)
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
    default:        return "";
    }
}