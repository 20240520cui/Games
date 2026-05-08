#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "configs/models/CardTypeDef.h"
#include <cocos2d.h>

class CardModel
{
public:
    CardModel();
    CardModel(int cardId, CardFaceType face, CardSuitType suit, float posX, float posY);

    int getId() const;
    CardFaceType getFace() const;
    CardSuitType getSuit() const;
    cocos2d::Vec2 getPosition() const;
    bool isFaceUp() const;

    void setId(int id);
    void setFace(CardFaceType face);
    void setSuit(CardSuitType suit);
    void setPosition(const cocos2d::Vec2& pos);
    void setFaceUp(bool faceUp);

    bool isRedSuit() const;
    bool isBlackSuit() const;

private:
    int _id;
    CardFaceType _face;
    CardSuitType _suit;
    cocos2d::Vec2 _position;
    bool _isFaceUp;
};

#endif