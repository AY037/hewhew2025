#pragma once
#include <nlohmann/json.hpp>
#include <fstream>           // �t�@�C������
#include <unordered_map>
#include "GameObject.h"
#include "GameObjectManager.h"

class SaveLoad {
public:
    // �V�[����ۑ�
    void SaveScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects);

    // �V�[����ǂݍ���
    void LoadScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects, std::vector<std::shared_ptr<GameObject>*>& objectList);

    // �V�[���̃��Z�b�g
    void ResetScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects);
};


