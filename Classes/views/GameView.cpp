#include "views/GameView.h"
#include "views/CardView.h"
#include "ui/UIButton.h"
#include "models/CardModel.h"

using namespace cocos2d;

GameView* GameView::create()
{
    GameView* view = new GameView();
    if (view->init())
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init()
{
    if (!Node::init()) return false;

    _cardViews.clear();

    _playfieldArea = Node::create();
    _playfieldArea->setPosition(Vec2(0, 0));
    addChild(_playfieldArea);

    _stackArea = Node::create();
    _stackArea->setPosition(Vec2(0, 700));
    addChild(_stackArea);

    auto bg = DrawNode::create();
    Vec2 pfPoints[4] = {Vec2(0, 0), Vec2(540, 0), Vec2(540, 700), Vec2(0, 700)};
    Color4F pfColor(0.2f, 0.6f, 0.2f, 0.3f);
    bg->drawPolygon(pfPoints, 4, pfColor, 0, Color4F::BLACK);
    _playfieldArea->addChild(bg);

    auto bgStack = DrawNode::create();
    Vec2 stPoints[4] = {Vec2(0, 0), Vec2(540, 0), Vec2(540, 260), Vec2(0, 260)};
    Color4F stColor(0.3f, 0.3f, 0.7f, 0.3f);
    bgStack->drawPolygon(stPoints, 4, stColor, 0, Color4F::BLACK);
    _stackArea->addChild(bgStack);

    _resultLabel = nullptr;
    _nextLevelButton = nullptr;
    _drawButton = nullptr;
    _baseCardId = -1;

    return true;
}

void GameView::addPlayfieldCard(int cardId, CardFaceType face, CardSuitType suit,
                                const Vec2& pos, bool isFaceUp,
                                const std::function<void(int)>& onClickCallback)
{
    auto cardView = CardView::create(cardId, face, suit, isFaceUp);
    if (cardView)
    {
        cardView->setPosition(pos);
        cardView->setClickCallback(onClickCallback);
        _playfieldArea->addChild(cardView);
        _cardViews[cardId] = cardView;
    }
}

void GameView::addStackCard(int cardId, CardFaceType face, CardSuitType suit,
                            const Vec2& pos, bool isFaceUp, int zOrder,
                            const std::function<void(int)>& onClickCallback)
{
    auto cardView = CardView::create(cardId, face, suit, isFaceUp);
    if (cardView)
    {
        cardView->setPosition(pos);
        cardView->setClickCallback(onClickCallback);
        _stackArea->addChild(cardView, zOrder);
        _cardViews[cardId] = cardView;
    }
}

CardView* GameView::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end()) return it->second;
    return nullptr;
}

void GameView::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

void GameView::updateBaseCard(int baseCardId)
{
    _baseCardId = baseCardId;

    if (baseCardId < 0) return;

    auto cardView = getCardView(baseCardId);
    if (cardView)
    {
        cardView->setPosition(Vec2(270, 130));
        _stackArea->reorderChild(cardView, 50);
    }
}

Node* GameView::getPlayfieldArea()
{
    return _playfieldArea;
}

Node* GameView::getStackArea()
{
    return _stackArea;
}

void GameView::setupUndoButton(const std::function<void()>& callback)
{
    _undoButton = ui::Button::create("res/card_general.png", "res/card_general.png");
    if (_undoButton)
    {
        _undoButton->setTitleText("Undo");
        _undoButton->setTitleFontSize(24);
        _undoButton->setPosition(Vec2(80, 220));
        _undoButton->addClickEventListener([callback](Ref*) {
            if (callback) callback();
        });
        _undoButton->setVisible(false);
        _stackArea->addChild(_undoButton, 100);
    }
}

void GameView::showUndoButton()
{
    if (_undoButton)
    {
        _undoButton->setVisible(true);
    }
}

void GameView::hideUndoButton()
{
    if (_undoButton)
    {
        _undoButton->setVisible(false);
    }
}

void GameView::setupDrawButton(const std::function<void()>& callback)
{
    _drawButton = ui::Button::create("res/card_general.png", "res/card_general.png");
    if (_drawButton)
    {
        _drawButton->setTitleText("Draw");
        _drawButton->setTitleFontSize(20);
        _drawButton->setPosition(Vec2(460, 130));
        _drawButton->addClickEventListener([callback](Ref*) {
            if (callback) callback();
        });
        _stackArea->addChild(_drawButton, 100);
    }
}

void GameView::showGameResult(bool isWin)
{
    if (_resultLabel)
    {
        _resultLabel->removeFromParent();
        _resultLabel = nullptr;
    }

    std::string text = isWin ? "You Win!" : "No More Matches!";
    _resultLabel = Label::createWithSystemFont(text, "Arial", 36);
    if (_resultLabel)
    {
        _resultLabel->setColor(isWin ? Color3B(255, 215, 0) : Color3B(255, 80, 80));
        _resultLabel->setPosition(Vec2(270, 350));
        addChild(_resultLabel, 200);
    }
}

void GameView::showNextLevelButton(const std::function<void()>& callback)
{
    if (_nextLevelButton)
    {
        _nextLevelButton->removeFromParent();
        _nextLevelButton = nullptr;
    }

    _nextLevelButton = ui::Button::create("res/card_general.png", "res/card_general.png");
    if (_nextLevelButton)
    {
        _nextLevelButton->setTitleText("Next Level");
        _nextLevelButton->setTitleFontSize(20);
        _nextLevelButton->setPosition(Vec2(270, 280));
        _nextLevelButton->addClickEventListener([callback](Ref*) {
            if (callback) callback();
        });
        addChild(_nextLevelButton, 200);
    }
}

void GameView::resetGame()
{
    removeAllChildren();
    _cardViews.clear();
    init();
}