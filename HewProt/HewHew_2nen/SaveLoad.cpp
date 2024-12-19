#include "SaveLoad.h"

// �V�[����ۑ�
void SaveLoad::SaveScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects) {
    nlohmann::json sceneJson;

    for (auto& pair : objects) {
        auto& obj = pair.second;

        DirectX::XMFLOAT3 pos = obj->GetPos();
        DirectX::XMFLOAT3 size = obj->GetSize();

        // GameObject �̃f�[�^�� JSON �I�u�W�F�N�g�Ƃ��Ēǉ�
        sceneJson["objects"].push_back({
            {"name", obj->GetName()},
            {"object_id", obj->GetObjID()},
            {"objectTexName", obj->GetObjectTexName()},
            {"position", {pos.x, pos.y, pos.z}},
            {"size", {size.x, size.y, size.z}},
            {"angle", obj->GetAngle()},
            });
    }

    // JSON �f�[�^���t�@�C���ɕۑ�
    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << sceneJson.dump(4); // �C���f���g 4 �Ő��`���ĕۑ�
        outFile.close();
    }
    else {
        throw std::runtime_error("Failed to open file for writing.");
    }
}

// �V�[����ǂݍ���
void SaveLoad::LoadScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects, std::vector<std::shared_ptr<GameObject>*>& objectList, TextureManager& textureManager) {
    nlohmann::json sceneJson;

    // �t�@�C����ǂݍ���
    std::ifstream inFile(fileName);
    if (inFile.is_open()) {
        inFile >> sceneJson;
        inFile.close();
    }
    else {
        throw std::runtime_error("Failed to open file for reading.");
    }

    // JSON �f�[�^���I�u�W�F�N�g�Ƃ��ĕ���
    for (const auto& objJson : sceneJson["objects"]) {
        std::shared_ptr<GameObject> tmpObj = GameObjectManager::GetInstance().GetObj(objJson["name"]);
        tmpObj->SetName(objJson["name"]);
        int objID = objectList.size();
        tmpObj->SetObjID(objID);

        auto pos = objJson["position"];
        tmpObj->SetPos(pos[0], pos[1], pos[2]);
        auto size = objJson["size"];
        tmpObj->SetSize(size[0], size[1], size[2]);
        tmpObj->SetAngle(objJson["angle"]);
        tmpObj->SetObjectTexName(objJson["objectTexName"]);
        objects.insert({ objID ,tmpObj });
        objectList.push_back(&objects[objID]);
    }
}

void SaveLoad::ResetScene(const std::string& fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& objects) {
    nlohmann::json sceneJson;

    // �t�@�C����ǂݍ���
    std::ifstream inFile(fileName);
    if (inFile.is_open()) {
        inFile >> sceneJson;
        inFile.close();
    }
    else {
        throw std::runtime_error("Failed to open file for reading.");
    }

    for (const auto& iter : objects)
    {
        auto& obj = iter.second;
        for (const auto& objJson : sceneJson["objects"])
        {
            if (obj->GetObjID() == objJson["object_id"])
            {
                auto pos = objJson["position"];
                obj->SetPos(pos[0], pos[1], pos[2]);
                auto size = objJson["size"];
                obj->SetSize(size[0], size[1], size[2]);
                obj->SetAngle(objJson["angle"]);
                obj->SetName(objJson["name"]);
            }
        }
    }
}