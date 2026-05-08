#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "models/CardModel.h"
#include <vector>
#include <map>
#include <set>

class GameModel
{
public:
    GameModel();

    const std::vector<CardModel>& getPlayfieldCards() const;
    const std::vector<CardModel>& getStackCards() const;
    int getTopStackCardId() const;
    const CardModel* getTopStackCard() const;
    bool isCardInPlayfield(int cardId) const;
    bool isCardTopOfStack(int cardId) const;
    const CardModel* getCardById(int id) const;
    bool isCardUncovered(int cardId) const;

    void setPlayfieldCards(const std::vector<CardModel>& cards);
    void setStackCards(const std::vector<CardModel>& cards);
    void setCoverageMap(const std::map<int, std::vector<int>>& coverage);
    void addPlayfieldCard(const CardModel& card);
    void addStackCardBack(const CardModel& card);

    void removePlayfieldCard(int cardId);
    void removeStackCard(int cardId);
    void flipNextStackCard();
    void unflipTopStackCard(int cardId);
    void flipPlayfieldCard(int cardId);
    void unflipPlayfieldCard(int cardId);
    void updatePlayfieldCardPosition(int cardId, const cocos2d::Vec2& pos);

private:
    std::vector<CardModel> _playfieldCards;
    std::vector<CardModel> _stackCards;
    std::map<int, CardModel> _allCards;
    std::map<int, std::vector<int>> _coverageMap;
};

#endif