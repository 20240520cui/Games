#include "controllers/GameController.h"
#include "controllers/CardController.h"
#include "views/GameView.h"
#include "views/CardView.h"
#include "services/GameModelFromLevelGenerator.h"
#include "services/CardMatchService.h"
#include "models/CardModel.h"
#include <string>
#include "configs/loaders/LevelConfigLoader.h"

using namespace cocos2d;

GameController* GameController::create()
{
    GameController* ctrl = new GameController();
    if (ctrl->init())
    {
        ctrl->autorelease();
        return ctrl;
    }
    CC_SAFE_DELETE(ctrl);
    return nullptr;
}

bool GameController::init()
{
    _gameView = nullptr;
    _cardController = nullptr;
    _scene = nullptr;
    _selectedCardId = -1;
    _isAnimating = false;
    _currentLevelId = 1;
    return true;
}

Scene* GameController::getScene()
{
    if (!_scene)
    {
        _scene = Scene::create();
    }
    return _scene;
}

void GameController::startGame(int levelId)
{
    _currentLevelId = levelId;
    std::string levelFile = "data/level_" + std::to_string(levelId) + ".json";
    LevelConfig config = LevelConfigLoader::loadFromFile(levelFile);

    GameModelFromLevelGenerator::generateGameModel(config, _gameModel);

    _cardController = new CardController();
    _cardController->init(&_gameModel, &_undoManager);

    _initView();
}

void GameController::_initView()
{
    _gameView = GameView::create();
    if (!_gameView) return;

    getScene()->addChild(_gameView);

    auto& playfieldCards = _gameModel.getPlayfieldCards();
    for (const auto& card : playfieldCards)
    {
        _gameView->addPlayfieldCard(card.getId(), card.getFace(), card.getSuit(),
            card.getPosition(), card.isFaceUp(),
            [this](int cardId) { handleCardClick(cardId); });
    }

    auto& stackCards = _gameModel.getStackCards();
    for (size_t i = 0; i < stackCards.size(); ++i)
    {
        const auto& card = stackCards[i];
        _gameView->addStackCard(card.getId(), card.getFace(), card.getSuit(),
            card.getPosition(), card.isFaceUp(), static_cast<int>(i),
            [this](int cardId) { handleCardClick(cardId); });
    }

    _gameView->setupUndoButton([this]() { handleUndoClick(); });
    _gameView->setupDrawButton([this]() { handleDrawClick(); });
    _gameView->updateBaseCard(_gameModel.getTopStackCardId());
}

void GameController::_updateView()
{
}

void GameController::handleCardClick(int cardId)
{
    if (!_gameView || _isAnimating) return;

    bool isInPlayfield = _gameModel.isCardInPlayfield(cardId);
    bool isInStack = _gameModel.isCardTopOfStack(cardId);

    if (!isInPlayfield && !isInStack) return;

    if (isInPlayfield)
    {
        const CardModel* clickedCard = _gameModel.getCardById(cardId);
        if (!clickedCard || !clickedCard->isFaceUp()) return;
        if (!_gameModel.isCardUncovered(cardId)) return;

        if (_selectedCardId < 0)
        {
            _selectCard(cardId);
        }
        else if (cardId == _selectedCardId)
        {
            _deselectCard();
        }
        else
        {
            _deselectCard();
            _selectCard(cardId);
        }
    }
    else if (isInStack)
    {
        if (_selectedCardId >= 0)
        {
            _tryMatchWithBase(_selectedCardId);
        }
    }
}

void GameController::_selectCard(int cardId)
{
    _selectedCardId = cardId;
    auto cardView = _gameView->getCardView(cardId);
    if (cardView)
    {
        cardView->setSelected(true);
    }
}

void GameController::_deselectCard()
{
    if (_selectedCardId >= 0)
    {
        auto cardView = _gameView->getCardView(_selectedCardId);
        if (cardView)
        {
            cardView->setSelected(false);
        }
    }
    _selectedCardId = -1;
}

void GameController::_tryMatchWithBase(int playfieldCardId)
{
    const CardModel* playfieldCard = _gameModel.getCardById(playfieldCardId);
    const CardModel* baseCard = _gameModel.getTopStackCard();

    if (!playfieldCard || !baseCard)
    {
        _deselectCard();
        return;
    }

    if (CardMatchService::canMatch(playfieldCard->getFace(), baseCard->getFace()))
    {
        _removeMatchedCard(playfieldCardId);
    }
    else
    {
        _deselectCard();
    }
}

void GameController::_removeMatchedCard(int playfieldCardId)
{
    _isAnimating = true;

    const CardModel* playfieldCard = _gameModel.getCardById(playfieldCardId);
    int baseCardId = _gameModel.getTopStackCardId();
    const CardModel* baseCard = _gameModel.getCardById(baseCardId);

    CardModel newBaseCard(playfieldCardId,
        playfieldCard->getFace(), playfieldCard->getSuit(),
        _gameModel.getTopStackCard()->getPosition().x,
        _gameModel.getTopStackCard()->getPosition().y);
    newBaseCard.setFaceUp(true);

    _gameModel.removePlayfieldCard(playfieldCardId);
    _gameModel.removeStackCard(baseCardId);
    _gameModel.addStackCardBack(newBaseCard);

    auto playfieldView = _gameView->getCardView(playfieldCardId);
    if (playfieldView)
    {
        playfieldView->setSelected(false);
        auto scaleUp = ScaleTo::create(0.15f, 1.3f);
        auto scaleDown = ScaleTo::create(0.15f, 0.0f);
        auto remove = CallFunc::create([this, playfieldCardId]() {
            _gameView->removeCardView(playfieldCardId);
        });
        playfieldView->runAction(Sequence::create(scaleUp, scaleDown, remove, nullptr));
    }

    auto baseView = _gameView->getCardView(baseCardId);
    if (baseView)
    {
        auto fadeOut = FadeOut::create(0.2f);
        auto removeBase = CallFunc::create([this, baseCardId]() {
            _gameView->removeCardView(baseCardId);
        });
        baseView->runAction(Sequence::create(fadeOut, removeBase, nullptr));
    }

    Director::getInstance()->getScheduler()->schedule([this, playfieldCardId, newBaseCard](float dt) {
        int zOrder = static_cast<int>(_gameModel.getStackCards().size()) - 1;
        _gameView->addStackCard(playfieldCardId, newBaseCard.getFace(), newBaseCard.getSuit(),
            newBaseCard.getPosition(), true, zOrder,
            [this](int cardId) { handleCardClick(cardId); });

        _gameView->updateBaseCard(_gameModel.getTopStackCardId());

        std::vector<int> revealedIds;
        _revealCoveredCards(playfieldCardId, revealedIds);

        _isAnimating = false;
        _checkGameEnd();
    }, this, 0.0f, 0, 0.35f, false, "matchFinish");

    _selectedCardId = -1;
}

void GameController::_revealCoveredCards(int removedCardId, std::vector<int>& revealedIds)
{
    auto& playfieldCards = _gameModel.getPlayfieldCards();
    for (const auto& card : playfieldCards)
    {
        if (!card.isFaceUp() && _gameModel.isCardUncovered(card.getId()))
        {
            _gameModel.flipPlayfieldCard(card.getId());
            _revealCoveredCardView(card.getId());
            revealedIds.push_back(card.getId());
        }
    }
}

void GameController::_revealCoveredCardView(int cardId)
{
    auto cardView = _gameView->getCardView(cardId);
    if (cardView)
    {
        cardView->setCardFaceUp(true);
    }
}

void GameController::_flipTopStackCardView()
{
    int topId = _gameModel.getTopStackCardId();
    if (topId < 0) return;

    const CardModel* topCard = _gameModel.getCardById(topId);
    if (!topCard) return;

    auto cardView = _gameView->getCardView(topId);
    if (cardView)
    {
        cardView->setCardFaceUp(true);
    }

    _gameView->updateBaseCard(_gameModel.getTopStackCardId());
}

void GameController::_checkGameEnd()
{
    auto& playfieldCards = _gameModel.getPlayfieldCards();

    if (playfieldCards.empty())
    {
        _gameView->showGameResult(true);
        _gameView->showNextLevelButton([this]() {
            _gameModel = GameModel();
            _undoManager = UndoManager();
            _gameView->resetGame();
            startGame(_currentLevelId + 1);
        });
        return;
    }

    const CardModel* baseCard = _gameModel.getTopStackCard();

    bool hasUnflippedStack = false;
    for (const auto& card : _gameModel.getStackCards())
    {
        if (!card.isFaceUp()) { hasUnflippedStack = true; break; }
    }

    bool canMatch = CardMatchService::hasAnyMatch(playfieldCards, _gameModel.getTopStackCardId(), baseCard);

    if (!canMatch && !hasUnflippedStack)
    {
        _gameView->showGameResult(false);
    }
}

void GameController::handleDrawClick()
{
    if (_isAnimating) return;

    _deselectCard();

    bool hasUnflippedStack = false;
    int nextFlippedId = -1;
    for (int i = static_cast<int>(_gameModel.getStackCards().size()) - 1; i >= 0; --i)
    {
        const auto& card = _gameModel.getStackCards()[i];
        if (!card.isFaceUp()) {
            hasUnflippedStack = true;
            nextFlippedId = card.getId();
            break;
        }
    }

    if (!hasUnflippedStack) return;

    int baseCardId = _gameModel.getTopStackCardId();
    const CardModel* baseCard = _gameModel.getCardById(baseCardId);

    UndoRecord drawRecord;
    drawRecord.actionType = UAT_DRAW_CARD;
    drawRecord.targetCardId = baseCardId;
    drawRecord.originalPosition = baseCard ? baseCard->getPosition() : Vec2(270, 130);
    drawRecord.isFromPlayfield = false;
    drawRecord.isFromStack = true;
    drawRecord.cardFace = baseCard ? baseCard->getFace() : CFT_NONE;
    drawRecord.cardSuit = baseCard ? baseCard->getSuit() : CST_NONE;
    drawRecord.flippedCardId = nextFlippedId;
    _undoManager.addRecord(drawRecord);

    _gameView->showUndoButton();

    _gameModel.removeStackCard(baseCardId);
    _gameView->removeCardView(baseCardId);

    _gameModel.flipNextStackCard();
    _flipTopStackCardView();
    _checkGameEnd();
}

void GameController::handleUndoClick()
{
    if (_isAnimating) return;
    if (!_undoManager.hasUndoRecords()) return;

    _deselectCard();

    UndoRecord record = _undoManager.popRecord();

    if (record.actionType == UAT_DRAW_CARD)
    {
        CardModel restoredCard(record.targetCardId, record.cardFace, record.cardSuit,
            record.originalPosition.x, record.originalPosition.y);
        restoredCard.setFaceUp(true);
        _gameModel.addStackCardBack(restoredCard);

        if (record.flippedCardId >= 0)
        {
            _gameModel.unflipTopStackCard(record.flippedCardId);

            auto belowView = _gameView->getCardView(record.flippedCardId);
            if (belowView) belowView->setCardFaceUp(false);
        }

        int zOrder = static_cast<int>(_gameModel.getStackCards().size()) - 1;
        _gameView->addStackCard(record.targetCardId, record.cardFace, record.cardSuit,
            record.originalPosition, true, zOrder,
            [this](int cardId) { handleCardClick(cardId); });

        _gameView->updateBaseCard(_gameModel.getTopStackCardId());
    }
}