#include "models/GameModel.h"

GameModel::GameModel()
{
}

const std::vector<CardModel>& GameModel::getPlayfieldCards() const
{
    return _playfieldCards;
}

const std::vector<CardModel>& GameModel::getStackCards() const
{
    return _stackCards;
}

int GameModel::getTopStackCardId() const
{
    if (_stackCards.empty()) return -1;
    return _stackCards.back().getId();
}

const CardModel* GameModel::getTopStackCard() const
{
    if (_stackCards.empty()) return nullptr;
    return &_stackCards.back();
}

bool GameModel::isCardInPlayfield(int cardId) const
{
    for (const auto& card : _playfieldCards)
    {
        if (card.getId() == cardId) return true;
    }
    return false;
}

bool GameModel::isCardTopOfStack(int cardId) const
{
    if (_stackCards.empty()) return false;
    return _stackCards.back().getId() == cardId;
}

const CardModel* GameModel::getCardById(int id) const
{
    auto it = _allCards.find(id);
    if (it != _allCards.end()) return &(it->second);
    return nullptr;
}

bool GameModel::isCardUncovered(int cardId) const
{
    auto it = _coverageMap.find(cardId);
    if (it == _coverageMap.end()) return true;

    for (int covererId : it->second)
    {
        if (isCardInPlayfield(covererId))
        {
            return false;
        }
    }

    return true;
}

void GameModel::setPlayfieldCards(const std::vector<CardModel>& cards)
{
    _playfieldCards = cards;
    for (const auto& card : cards)
    {
        _allCards[card.getId()] = card;
    }
}

void GameModel::setStackCards(const std::vector<CardModel>& cards)
{
    _stackCards = cards;
    for (const auto& card : cards)
    {
        _allCards[card.getId()] = card;
    }
}

void GameModel::setCoverageMap(const std::map<int, std::vector<int>>& coverage)
{
    _coverageMap = coverage;
}

void GameModel::addPlayfieldCard(const CardModel& card)
{
    _playfieldCards.push_back(card);
    _allCards[card.getId()] = card;
}

void GameModel::addStackCardBack(const CardModel& card)
{
    _stackCards.push_back(card);
    _allCards[card.getId()] = card;
}

void GameModel::removePlayfieldCard(int cardId)
{
    _playfieldCards.erase(
        std::remove_if(_playfieldCards.begin(), _playfieldCards.end(),
            [cardId](const CardModel& c) { return c.getId() == cardId; }),
        _playfieldCards.end());

    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        _allCards.erase(it);
    }
}

void GameModel::removeStackCard(int cardId)
{
    _stackCards.erase(
        std::remove_if(_stackCards.begin(), _stackCards.end(),
            [cardId](const CardModel& c) { return c.getId() == cardId; }),
        _stackCards.end());

    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        _allCards.erase(it);
    }
}

void GameModel::flipNextStackCard()
{
    if (_stackCards.empty()) return;

    for (int i = static_cast<int>(_stackCards.size()) - 1; i >= 0; --i)
    {
        if (!_stackCards[i].isFaceUp())
        {
            _stackCards[i].setFaceUp(true);
            auto it = _allCards.find(_stackCards[i].getId());
            if (it != _allCards.end())
            {
                it->second.setFaceUp(true);
            }
            return;
        }
    }
}

void GameModel::unflipTopStackCard(int cardId)
{
    for (auto& card : _stackCards)
    {
        if (card.getId() == cardId)
        {
            card.setFaceUp(false);
            auto it = _allCards.find(cardId);
            if (it != _allCards.end())
            {
                it->second.setFaceUp(false);
            }
            return;
        }
    }
}

void GameModel::flipPlayfieldCard(int cardId)
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        it->second.setFaceUp(true);
        for (auto& card : _playfieldCards)
        {
            if (card.getId() == cardId)
            {
                card.setFaceUp(true);
                break;
            }
        }
    }
}

void GameModel::unflipPlayfieldCard(int cardId)
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        it->second.setFaceUp(false);
        for (auto& card : _playfieldCards)
        {
            if (card.getId() == cardId)
            {
                card.setFaceUp(false);
                break;
            }
        }
    }
}

void GameModel::updatePlayfieldCardPosition(int cardId, const cocos2d::Vec2& pos)
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        it->second.setPosition(pos);
        for (auto& card : _playfieldCards)
        {
            if (card.getId() == cardId)
            {
                card.setPosition(pos);
                break;
            }
        }
    }
}