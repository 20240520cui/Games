#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/UndoModel.h"
#include "models/GameModel.h"

class GameView;

class UndoManager
{
public:
    UndoManager();

    void addRecord(const UndoRecord& record);
    UndoRecord popRecord();
    bool hasUndoRecords() const;

private:
    UndoModel _undoModel;
};

#endif