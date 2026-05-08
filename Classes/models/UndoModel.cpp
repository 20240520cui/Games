#include "models/UndoModel.h"

UndoModel::UndoModel()
{
}

void UndoModel::pushRecord(const UndoRecord& record)
{
    _records.push_back(record);
}

UndoRecord UndoModel::popRecord()
{
    if (_records.empty())
    {
        UndoRecord empty;
        empty.actionType = UAT_NUM_TYPES;
        empty.targetCardId = -1;
        empty.originalPosition = cocos2d::Vec2(0, 0);
        empty.isFromPlayfield = false;
        empty.isFromStack = false;
        empty.cardFace = CFT_NONE;
        empty.cardSuit = CST_NONE;
        empty.previousBaseCardId = -1;
        empty.previousBaseCardFace = CFT_NONE;
        empty.previousBaseCardSuit = CST_NONE;
        empty.previousBaseCardPosition = cocos2d::Vec2(0, 0);
        empty.wasFaceUp = false;
        empty.revealedCardIds.clear();
        empty.flippedCardId = -1;
        return empty;
    }

    UndoRecord record = _records.back();
    _records.pop_back();
    return record;
}

bool UndoModel::hasRecords() const
{
    return !_records.empty();
}

void UndoModel::clear()
{
    _records.clear();
}