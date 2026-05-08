#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "configs/models/CardTypeDef.h"
#include <vector>

struct CardConfig
{
    int cardFace;
    int cardSuit;
    float posX;
    float posY;
    std::vector<int> coveredBy;
};

struct LevelConfig
{
    std::vector<CardConfig> playfield;
    std::vector<CardConfig> stack;
};

#endif