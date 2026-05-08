#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "configs/models/CardTypeDef.h"
#include <functional>

class CardView : public cocos2d::Node
{
public:
    static CardView* create(int cardId, CardFaceType face, CardSuitType suit, bool isFaceUp);

    virtual bool init(int cardId, CardFaceType face, CardSuitType suit, bool isFaceUp);

    void setCardFaceUp(bool faceUp);
    void setSelected(bool selected);
    bool isSelected() const;
    void setClickCallback(const std::function<void(int)>& callback);
    int getCardId() const;
    CardFaceType getCardFace() const;
    CardSuitType getCardSuit() const;

    void playMoveToAnimation(const cocos2d::Vec2& targetPos, float duration = 0.3f,
                              const std::function<void()>& onComplete = nullptr);

    static const float kCardWidth;
    static const float kCardHeight;

private:
    void _setupTouchListener();
    void _updateVisual();

    int _cardId;
    CardFaceType _face;
    CardSuitType _suit;
    bool _isFaceUp;
    bool _isSelected;
    std::function<void(int)> _onClickCallback;
    cocos2d::Sprite* _cardBgSprite;
    cocos2d::Sprite* _cardBackSprite;
    cocos2d::DrawNode* _backOverlay;
    cocos2d::Sprite* _bigNumberSprite;
    cocos2d::Sprite* _smallNumberSprite;
    cocos2d::Sprite* _suitSprite;
    cocos2d::Sprite* _centerSuitSprite;
    cocos2d::DrawNode* _selectionOutline;
};

#endif