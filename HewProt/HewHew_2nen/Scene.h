#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <queue>
#include "GameObject.h"
#include "Sound.h"
#include "Camera.h"
#include "SaveLoad.h"

class Scene {
public:
    Scene() : camera(nullptr) {} // camera�̏�����
    virtual ~Scene() {
    }

    virtual void Init() = 0;    // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
    virtual void Update() = 0;  // �V�[�����̃I�u�W�F�N�g�X�V�B
    virtual void Draw() = 0;    // �V�[�����̃I�u�W�F�N�g��`��
    virtual void Shutdown() {}  // �V�[���̏I�������B

    void EngineUpdate();
    //void SetNameToIDs();

    //�I�u�W�F�N�g�̒ǉ��Ə���
    void AddObject(std::shared_ptr<GameObject>&_gameObject);
    void AddRemoveObject(int);
    void DeleteObject(int _ObjectID);

    //�Q�b�^�[
    const TextureManager& GetTextureManager();
    std::unordered_map<int, std::shared_ptr<GameObject>>* GetGameObjects();
    const DirectX::XMMATRIX& GetViewMatrix();
    const DirectX::XMMATRIX& GetProjectionMatrix();
    const std::unique_ptr<Camera>& GetCamera(){ return camera; }
    std::vector<int> FindObjID(const std::string& objName);
    SaveLoad& GetSaveLoad();

protected:
    std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
    std::vector<std::shared_ptr<GameObject>*> gameObjectList; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
    std::queue<int> availableIDs;//�����ꂽ�I�u�W�F�N�g��ID�̃L���[FIFO
    //std::unordered_map<std::string, std::vector<int>> nameToIDs; // �I�u�W�F�N�g���������I�u�W�F�N�g��ID������
    std::vector<int> addObjects;//�ǉ��\��̃I�u�W�F�N�g���X�g
    std::vector<int> removeObjects;//�����\��̃I�u�W�F�N�g���X�g
    TextureManager& textureManager = TextureManager::GetInstance();
    SaveLoad saveload;
    std::unique_ptr<Camera> camera; // �V�[�����Ƃ̃J����
    Sound& sound = Sound::GetInstance();
    DirectX::XMMATRIX vm;
    DirectX::XMMATRIX pm;
};


