#include "models/CardModel.h"

CardModel::CardModel()
    : _id(-1)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _position(0, 0)
    , _isFaceUp(false)
{
}

CardModel::CardModel(int cardId, CardFaceType face, CardSuitType suit, float posX, float posY)
    : _id(cardId)
    , _face(face)
    , _suit(suit)
    , _position(posX, posY)
    , _isFaceUp(true)
{
}

int CardModel::getId() const { return _id; }
CardFaceType CardModel::getFace() const { return _face; }
CardSuitType CardModel::getSuit() const { return _suit; }
cocos2d::Vec2 CardModel::getPosition() const { return _position; }
bool CardModel::isFaceUp() const { return _isFaceUp; }

void CardModel::setId(int id) { _id = id; }
void CardModel::setFace(CardFaceType face) { _face = face; }
void CardModel::setSuit(CardSuitType suit) { _suit = suit; }
void CardModel::setPosition(const cocos2d::Vec2& pos) { _position = pos; }
void CardModel::setFaceUp(bool faceUp) { _isFaceUp = faceUp; }

bool CardModel::isRedSuit() const
{
    return _suit == CST_HEARTS || _suit == CST_DIAMONDS;
}

bool CardModel::isBlackSuit() const
{
    return _suit == CST_CLUBS || _suit == CST_SPADES;
}