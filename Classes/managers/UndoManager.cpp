#include "managers/UndoManager.h"

UndoManager::UndoManager()
{
}

void UndoManager::addRecord(const UndoRecord& record)
{
    _undoModel.clear();
    _undoModel.pushRecord(record);
}

UndoRecord UndoManager::popRecord()
{
    return _undoModel.popRecord();
}

bool UndoManager::hasUndoRecords() const
{
    return _undoModel.hasRecords();
}