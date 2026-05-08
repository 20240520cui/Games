#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "configs/models/CardTypeDef.h"
#include <string>
#include <map>

class CardResConfig
{
public:
    static const std::string& getNumberResPath(CardFaceType face, bool isBig, bool isBlack);
    static const std::string& getSuitResPath(CardSuitType suit);
    static const std::string& getCardGeneralResPath();

    static std::string buildNumberResName(CardFaceType face, bool isBig, bool isBlack);
    static std::string buildSuitResName(CardSuitType suit);

private:
    static std::string faceTypeToString(CardFaceType face);
};

#endif