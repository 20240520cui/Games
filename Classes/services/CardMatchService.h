#ifndef __CARD_MATCH_SERVICE_H__
#define __CARD_MATCH_SERVICE_H__

#include "configs/models/CardTypeDef.h"
#include <vector>

class CardModel;

class CardMatchService
{
public:
    static bool canMatch(CardFaceType faceA, CardFaceType faceB);
    static int getFaceValue(CardFaceType face);
    static bool hasAnyMatch(const std::vector<CardModel>& playfieldCards, int topStackCardId, const CardModel* topStackCard);
};

#endif