#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/LevelConfig.h"
#include "platform/CCFileUtils.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

LevelConfig LevelConfigLoader::loadFromFile(const std::string& filePath)
{
    LevelConfig config;

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(filePath);
    if (jsonStr.empty()) return config;

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    if (doc.HasParseError()) return config;

    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const auto& arr = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
        {
            const auto& item = arr[i];
            CardConfig cc;
            cc.cardFace = item.HasMember("CardFace") ? item["CardFace"].GetInt() : 0;
            cc.cardSuit = item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : 0;
            cc.posX = item.HasMember("Position") && item["Position"].HasMember("x") ? item["Position"]["x"].GetFloat() : 0.0f;
            cc.posY = item.HasMember("Position") && item["Position"].HasMember("y") ? item["Position"]["y"].GetFloat() : 0.0f;

            cc.coveredBy.clear();
            if (item.HasMember("CoveredBy") && item["CoveredBy"].IsArray())
            {
                const auto& cbArr = item["CoveredBy"];
                for (rapidjson::SizeType j = 0; j < cbArr.Size(); ++j)
                {
                    cc.coveredBy.push_back(cbArr[j].GetInt());
                }
            }

            config.playfield.push_back(cc);
        }
    }

    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const auto& arr = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i)
        {
            const auto& item = arr[i];
            CardConfig cc;
            cc.cardFace = item.HasMember("CardFace") ? item["CardFace"].GetInt() : 0;
            cc.cardSuit = item.HasMember("CardSuit") ? item["CardSuit"].GetInt() : 0;
            cc.posX = 0.0f;
            cc.posY = 0.0f;
            config.stack.push_back(cc);
        }
    }

    return config;
}