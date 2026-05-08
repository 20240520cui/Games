#include "views/CardView.h"
#include "configs/models/CardResConfig.h"
#include "utils/CardUtils.h"

using namespace cocos2d;

const float CardView::kCardWidth = 60.0f;
const float CardView::kCardHeight = 90.0f;

CardView* CardView::create(int cardId, CardFaceType face, CardSuitType suit, bool isFaceUp)
{
    CardView* view = new CardView();
    if (view->init(cardId, face, suit, isFaceUp))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(int cardId, CardFaceType face, CardSuitType suit, bool isFaceUp)
{
    if (!Node::init()) return false;

    _cardId = cardId;
    _face = face;
    _suit = suit;
    _isFaceUp = isFaceUp;
    _isSelected = false;
    _onClickCallback = nullptr;
    _cardBgSprite = nullptr;
    _cardBackSprite = nullptr;
    _backOverlay = nullptr;
    _bigNumberSprite = nullptr;
    _smallNumberSprite = nullptr;
    _suitSprite = nullptr;
    _centerSuitSprite = nullptr;
    _selectionOutline = nullptr;

    setContentSize(Size(kCardWidth, kCardHeight));

    _updateVisual();
    _setupTouchListener();

    return true;
}

void CardView::setCardFaceUp(bool faceUp)
{
    _isFaceUp = faceUp;
    _updateVisual();
}

void CardView::setSelected(bool selected)
{
    _isSelected = selected;
    if (_selectionOutline)
    {
        _selectionOutline->removeFromParent();
        _selectionOutline = nullptr;
    }
    if (_isSelected)
    {
        _selectionOutline = DrawNode::create();
        Vec2 outlineRect[4] = {
            Vec2(-kCardWidth / 2 - 2, -kCardHeight / 2 - 2),
            Vec2(kCardWidth / 2 + 2, -kCardHeight / 2 - 2),
            Vec2(kCardWidth / 2 + 2, kCardHeight / 2 + 2),
            Vec2(-kCardWidth / 2 - 2, kCardHeight / 2 + 2)
        };
        _selectionOutline->drawPolygon(outlineRect, 4, Color4F(0, 0, 0, 0), 2.0f, Color4F(1.0f, 0.85f, 0.0f, 1.0f));
        addChild(_selectionOutline, 100);
    }
}

bool CardView::isSelected() const
{
    return _isSelected;
}

void CardView::setClickCallback(const std::function<void(int)>& callback)
{
    _onClickCallback = callback;
}

int CardView::getCardId() const
{
    return _cardId;
}

CardFaceType CardView::getCardFace() const
{
    return _face;
}

CardSuitType CardView::getCardSuit() const
{
    return _suit;
}

void CardView::playMoveToAnimation(const Vec2& targetPos, float duration,
                                    const std::function<void()>& onComplete)
{
    auto moveAction = MoveTo::create(duration, targetPos);
    if (onComplete)
    {
        auto callbackAction = CallFunc::create(onComplete);
        runAction(Sequence::create(moveAction, callbackAction, nullptr));
    }
    else
    {
        runAction(moveAction);
    }
}

void CardView::_setupTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        Vec2 location = touch->getLocation();
        Vec2 local = convertToNodeSpace(location);

        Size size = getContentSize();
        Rect bounds(-size.width / 2, -size.height / 2, size.width, size.height);
        if (bounds.containsPoint(local))
        {
            return true;
        }
        return false;
    };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        if (_onClickCallback && _isFaceUp)
        {
            _onClickCallback(_cardId);
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::_updateVisual()
{
    if (_cardBgSprite)
    {
        _cardBgSprite->removeFromParent();
        _cardBgSprite = nullptr;
    }
    if (_cardBackSprite)
    {
        _cardBackSprite->removeFromParent();
        _cardBackSprite = nullptr;
    }
    if (_backOverlay)
    {
        _backOverlay->removeFromParent();
        _backOverlay = nullptr;
    }
    if (_bigNumberSprite)
    {
        _bigNumberSprite->removeFromParent();
        _bigNumberSprite = nullptr;
    }
    if (_smallNumberSprite)
    {
        _smallNumberSprite->removeFromParent();
        _smallNumberSprite = nullptr;
    }
    if (_suitSprite)
    {
        _suitSprite->removeFromParent();
        _suitSprite = nullptr;
    }
    if (_centerSuitSprite)
    {
        _centerSuitSprite->removeFromParent();
        _centerSuitSprite = nullptr;
    }

    if (_isFaceUp)
    {
        _cardBgSprite = Sprite::create(CardResConfig::getCardGeneralResPath());
        if (_cardBgSprite)
        {
            _cardBgSprite->setScale(0.33f);
            _cardBgSprite->setPosition(Vec2(0, 0));
            addChild(_cardBgSprite);
        }

        bool isBlack = CardUtils::isRedSuit(_suit) ? false : true;

        const std::string& bigNumPath = CardResConfig::getNumberResPath(_face, true, isBlack);
        _bigNumberSprite = Sprite::create(bigNumPath);
        if (_bigNumberSprite)
        {
            _bigNumberSprite->setScale(0.35f);
            _bigNumberSprite->setPosition(Vec2(-kCardWidth / 2 + 12, kCardHeight / 2 - 12));
            addChild(_bigNumberSprite);
        }

        const std::string& smallNumPath = CardResConfig::getNumberResPath(_face, false, isBlack);
        _smallNumberSprite = Sprite::create(smallNumPath);
        if (_smallNumberSprite)
        {
            _smallNumberSprite->setScale(0.5f);
            _smallNumberSprite->setPosition(Vec2(kCardWidth / 2 - 12, -kCardHeight / 2 + 12));
            addChild(_smallNumberSprite);
        }

        const std::string& suitPath = CardResConfig::getSuitResPath(_suit);
        _suitSprite = Sprite::create(suitPath);
        if (_suitSprite)
        {
            _suitSprite->setScale(0.6f);
            _suitSprite->setPosition(Vec2(-kCardWidth / 2 + 12, kCardHeight / 2 - 28));
            addChild(_suitSprite);
        }

        _centerSuitSprite = Sprite::create(suitPath);
        if (_centerSuitSprite)
        {
            _centerSuitSprite->setScale(1.2f);
            _centerSuitSprite->setPosition(Vec2(0, -5));
            addChild(_centerSuitSprite);
        }
    }
    else
    {
        _cardBackSprite = Sprite::create(CardResConfig::getCardGeneralResPath());
        if (_cardBackSprite)
        {
            _cardBackSprite->setScale(0.33f);
            _cardBackSprite->setPosition(Vec2(0, 0));
            addChild(_cardBackSprite);
        }

        _backOverlay = DrawNode::create();
        Vec2 backRect[4] = {
            Vec2(-kCardWidth / 2, -kCardHeight / 2),
            Vec2(kCardWidth / 2, -kCardHeight / 2),
            Vec2(kCardWidth / 2, kCardHeight / 2),
            Vec2(-kCardWidth / 2, kCardHeight / 2)
        };
        Color4F backColor(0.2f, 0.2f, 0.5f, 0.85f);
        _backOverlay->drawPolygon(backRect, 4, backColor, 1.5f, Color4F(0.1f, 0.1f, 0.3f, 1.0f));
        addChild(_backOverlay);
    }
}