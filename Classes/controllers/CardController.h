#ifndef __CARD_CONTROLLER_H__
#define __CARD_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"

class GameView;

class CardController
{
public:
    CardController();
    ~CardController();

    void init(GameModel* gameModel, UndoManager* undoManager);

    bool handleCardClick(int cardId, GameView* gameView);

private:
    GameModel* _gameModel;
    UndoManager* _undoManager;
};

#endif