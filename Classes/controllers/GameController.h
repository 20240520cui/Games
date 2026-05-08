#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "managers/UndoManager.h"

class GameView;
class CardController;

class GameController : public cocos2d::Ref
{
public:
    static GameController* create();

    bool init();
    void startGame(int levelId);

    cocos2d::Scene* getScene();

    void handleCardClick(int cardId);
    void handleUndoClick();
    void handleDrawClick();

private:
    void _initView();
    void _updateView();
    void _selectCard(int cardId);
    void _deselectCard();
    void _tryMatchWithBase(int playfieldCardId);
    void _removeMatchedCard(int playfieldCardId);
    void _revealCoveredCards(int removedCardId, std::vector<int>& revealedIds);
    void _revealCoveredCardView(int cardId);
    void _flipTopStackCardView();
    void _checkGameEnd();
    void _updateBaseCardDisplay();

    GameModel _gameModel;
    UndoManager _undoManager;
    GameView* _gameView;
    CardController* _cardController;
    cocos2d::Scene* _scene;
    int _selectedCardId;
    bool _isAnimating;
    int _currentLevelId;
};

#endif