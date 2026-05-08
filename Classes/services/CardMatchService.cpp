#include "services/CardMatchService.h"
#include "models/CardModel.h"

bool CardMatchService::canMatch(CardFaceType faceA, CardFaceType faceB)
{
    int valA = getFaceValue(faceA);
    int valB = getFaceValue(faceB);

    int diff = valA - valB;
    return (diff == 1 || diff == -1 || diff == 12 || diff == -12);
}

int CardMatchService::getFaceValue(CardFaceType face)
{
    return static_cast<int>(face);
}

bool CardMatchService::hasAnyMatch(const std::vector<CardModel>& playfieldCards, int baseCardId, const CardModel* baseCard)
{
    if (!baseCard) return false;

    CardFaceType baseFace = baseCard->getFace();

    for (const auto& card : playfieldCards)
    {
        if (card.isFaceUp() && canMatch(card.getFace(), baseFace))
        {
            return true;
        }
    }

    return false;
}