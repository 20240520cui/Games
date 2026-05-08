#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

class GameModelFromLevelGenerator
{
public:
    static void generateGameModel(const LevelConfig& config, GameModel& outModel);
};

#endif