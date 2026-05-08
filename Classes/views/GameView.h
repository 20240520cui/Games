#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "ui/UIButton.h"
#include "configs/models/CardTypeDef.h"
#include "models/CardModel.h"
#include <functional>
#include <map>

class CardView;

class GameView : public cocos2d::Node
{
public:
    static GameView* create();

    virtual bool init();

    void addPlayfieldCard(int cardId, CardFaceType face, CardSuitType suit,
                          const cocos2d::Vec2& pos, bool isFaceUp,
                          const std::function<void(int)>& onClickCallback);
    void addStackCard(int cardId, CardFaceType face, CardSuitType suit,
                      const cocos2d::Vec2& pos, bool isFaceUp, int zOrder,
                      const std::function<void(int)>& onClickCallback);

    CardView* getCardView(int cardId);
    void removeCardView(int cardId);

    void updateBaseCard(int baseCardId);

    cocos2d::Node* getPlayfieldArea();
    cocos2d::Node* getStackArea();

    void setupUndoButton(const std::function<void()>& callback);
    void showUndoButton();
    void hideUndoButton();
    void setupDrawButton(const std::function<void()>& callback);
    void showGameResult(bool isWin);
    void showNextLevelButton(const std::function<void()>& callback);
    void resetGame();

private:
    std::map<int, CardView*> _cardViews;
    cocos2d::Node* _playfieldArea;
    cocos2d::Node* _stackArea;
    int _baseCardId;
    cocos2d::ui::Button* _undoButton;
    cocos2d::ui::Button* _drawButton;
    cocos2d::Label* _resultLabel;
    cocos2d::ui::Button* _nextLevelButton;
};

#endif