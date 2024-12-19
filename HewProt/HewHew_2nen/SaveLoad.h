#pragma once
#include <nlohmann/json.hpp>
#include <fstream>           // ファイル操作
#include <unordered_map>
#include "GameObject.h"
#include "GameObjectManager.h"

class SaveLoad {
public:
    // シーンを保存
    void SaveScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects);

    // シーンを読み込み
    void LoadScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects, std::vector<std::shared_ptr<GameObject>*>& objectList, TextureManager& textureManager);

    // シーンのリセット
    void ResetScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects);
};


