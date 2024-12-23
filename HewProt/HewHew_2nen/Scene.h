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
    Scene(){
    } // camera�̏�����
    virtual ~Scene() {
        for (auto& pair : gameObjects) {
            pair.second->Uninit();
        }
        gameObjects.clear(); // �x�N�^���N���A
        for (auto& obj : uiObjectList) {
            obj->Uninit();
        }
        uiObjectList.clear(); // �x�N�^���N���A
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
    void DeleteObject(const int);

    //�I�u�W�F�N�g�̒ǉ����X�g�Ə������X�g�̉����A�b�v�f�[�g�̍Ō�
    void AddAndDelete();

    //�Q�b�^�[
    const TextureManager& GetTextureManager();
    std::unordered_map<int, std::shared_ptr<GameObject>>* GetGameObjects();
    const DirectX::XMMATRIX& GetViewMatrix();
    const DirectX::XMMATRIX& GetProjectionMatrix();
    Camera& GetCamera(){ return camera; }
    std::vector<int> FindObjID(const std::string objName);
    SaveLoad& GetSaveLoad();

protected:
    std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
    std::vector<std::shared_ptr<GameObject>*> gameObjectList; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
    std::vector<std::shared_ptr<GameObject>> uiObjectList; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
    std::queue<int> availableIDs;//�����ꂽ�I�u�W�F�N�g��ID�̃L���[FIFO
    //std::unordered_map<std::string, std::vector<int>> nameToIDs; // �I�u�W�F�N�g���������I�u�W�F�N�g��ID������
    std::vector<int> addObjects;//�ǉ��\��̃I�u�W�F�N�g���X�g
    std::vector<int> removeObjects;//�����\��̃I�u�W�F�N�g���X�g
    TextureManager& textureManager = TextureManager::GetInstance();
    SaveLoad saveload;
    Camera camera; // �V�[�����Ƃ̃J����
    Sound& sound = Sound::GetInstance();
    DirectX::XMMATRIX vm;
    DirectX::XMMATRIX pm;
	//�ǂݍ��ރt�@�C����
	std::string sceneName;
};


