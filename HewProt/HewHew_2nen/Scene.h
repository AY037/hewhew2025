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
    Scene() : camera(nullptr) {} // cameraの初期化
    virtual ~Scene() {
    }

    virtual void Init() = 0;    // シーンの初期化。ここにオブジェクトを追加
    virtual void Update() = 0;  // シーン内のオブジェクト更新。
    virtual void Draw() = 0;    // シーン内のオブジェクトを描画
    virtual void Shutdown() {}  // シーンの終了処理。

    void EngineUpdate();
    //void SetNameToIDs();

    //オブジェクトの追加と消去
    void AddObject(std::shared_ptr<GameObject>&_gameObject);
    void DeleteObject(int _ObjectID);

    //ゲッター
    const TextureManager& GetTextureManager();
    std::unordered_map<int, std::shared_ptr<GameObject>>* GetGameObjects();
    const DirectX::XMMATRIX& GetViewMatrix();
    const DirectX::XMMATRIX& GetProjectionMatrix();
    const std::unique_ptr<Camera>& GetCamera(){ return camera; }
    std::vector<int> FindObjID(const std::string& objName);
    SaveLoad& GetSaveLoad();

protected:
    std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // シーン内のゲームオブジェクトリスト
    std::queue<int> availableIDs;//消されたオブジェクトのIDのキューFIFO
    //std::unordered_map<std::string, std::vector<int>> nameToIDs; // オブジェクト名が同じオブジェクトのIDを検索
    TextureManager& textureManager = TextureManager::GetInstance();
    SaveLoad saveload;
    std::unique_ptr<Camera> camera; // シーンごとのカメラ
    Sound& sound = Sound::GetInstance();
    DirectX::XMMATRIX vm;
    DirectX::XMMATRIX pm;
};


