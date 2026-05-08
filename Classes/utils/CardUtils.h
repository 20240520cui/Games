#ifndef __CARD_UTILS_H__
#define __CARD_UTILS_H__

#include "configs/models/CardTypeDef.h"
#include <string>

class CardUtils
{
public:
    static std::string suitToString(CardSuitType suit);
    static std::string faceToString(CardFaceType face);
    static bool isRedSuit(CardSuitType suit);
};

#endif