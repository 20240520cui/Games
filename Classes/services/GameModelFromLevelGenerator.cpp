#include "services/GameModelFromLevelGenerator.h"
#include "configs/models/CardTypeDef.h"
#include "views/CardView.h"

void GameModelFromLevelGenerator::generateGameModel(const LevelConfig& config, GameModel& outModel)
{
    std::vector<CardModel> playfieldCards;
    int cardId = 0;

    std::map<int, std::vector<int>> coverageMap;

    for (const auto& cc : config.playfield)
    {
        CardFaceType face = static_cast<CardFaceType>(cc.cardFace);
        CardSuitType suit = static_cast<CardSuitType>(cc.cardSuit);
        CardModel card(cardId, face, suit, cc.posX, cc.posY);

        bool uncovered = cc.coveredBy.empty();
        card.setFaceUp(uncovered);

        playfieldCards.push_back(card);

        if (!cc.coveredBy.empty())
        {
            coverageMap[cardId] = cc.coveredBy;
        }

        cardId++;
    }

    outModel.setPlayfieldCards(playfieldCards);
    outModel.setCoverageMap(coverageMap);

    std::vector<CardModel> stackCards;
    float stackPosX = 270.0f;
    float stackPosY = 130.0f;
    float stackOffsetY = 25.0f;

    for (size_t i = 0; i < config.stack.size(); ++i)
    {
        const auto& cc = config.stack[i];
        CardFaceType face = static_cast<CardFaceType>(cc.cardFace);
        CardSuitType suit = static_cast<CardSuitType>(cc.cardSuit);

        bool isFaceUp = (i == config.stack.size() - 1);
        CardModel card(cardId++, face, suit, stackPosX, stackPosY + i * stackOffsetY);
        card.setFaceUp(isFaceUp);
        stackCards.push_back(card);
    }

    outModel.setStackCards(stackCards);
}