#include "controllers/CardController.h"

CardController::CardController()
    : _gameModel(nullptr)
    , _undoManager(nullptr)
{
}

CardController::~CardController()
{
}

void CardController::init(GameModel* gameModel, UndoManager* undoManager)
{
    _gameModel = gameModel;
    _undoManager = undoManager;
}