#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include <cocos2d.h>
#include <vector>
#include "configs/models/CardTypeDef.h"

enum UndoActionType
{
    UAT_MATCH_CARD,
    UAT_DRAW_CARD,
    UAT_NUM_TYPES
};

struct UndoRecord
{
    UndoActionType actionType;
    int targetCardId;
    cocos2d::Vec2 originalPosition;
    bool isFromPlayfield;
    bool isFromStack;
    CardFaceType cardFace;
    CardSuitType cardSuit;
    bool wasFaceUp;

    int previousBaseCardId;
    CardFaceType previousBaseCardFace;
    CardSuitType previousBaseCardSuit;
    cocos2d::Vec2 previousBaseCardPosition;

    std::vector<int> revealedCardIds;

    int flippedCardId; // 只有 UAT_DRAW_CARD 用：记录翻出来的牌的 id
};

class UndoModel
{
public:
    UndoModel();

    void pushRecord(const UndoRecord& record);
    UndoRecord popRecord();
    bool hasRecords() const;
    void clear();

private:
    std::vector<UndoRecord> _records;
};

#endif