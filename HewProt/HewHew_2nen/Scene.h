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
    } // cameraの初期化
    virtual ~Scene() {
        for (auto& pair : gameObjects) {
            pair.second->Uninit();
        }
        gameObjects.clear(); // ベクタをクリア
        for (auto& obj : uiObjectList) {
            obj->Uninit();
        }
        uiObjectList.clear(); // ベクタをクリア
    }

    virtual void Init() = 0;    // シーンの初期化。ここにオブジェクトを追加
    virtual void Update() = 0;  // シーン内のオブジェクト更新。
    virtual void Draw() = 0;    // シーン内のオブジェクトを描画
    virtual void Shutdown() {}  // シーンの終了処理。

    void EngineUpdate();
    //void SetNameToIDs();

    //オブジェクトの追加と消去
    void AddObject(std::shared_ptr<GameObject>&_gameObject);
    void AddRemoveObject(int);
    void DeleteObject(const int);

    //オブジェクトの追加リストと消去リストの解決アップデートの最後
    void AddAndDelete();

    //ゲッター
    const TextureManager& GetTextureManager();
    std::unordered_map<int, std::shared_ptr<GameObject>>* GetGameObjects();
    const DirectX::XMMATRIX& GetViewMatrix();
    const DirectX::XMMATRIX& GetProjectionMatrix();
    Camera& GetCamera(){ return camera; }
    std::vector<int> FindObjID(const std::string objName);
    SaveLoad& GetSaveLoad();

protected:
    std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // シーン内のゲームオブジェクトリスト
    std::vector<std::shared_ptr<GameObject>*> gameObjectList; // シーン内のゲームオブジェクトリスト
    std::vector<std::shared_ptr<GameObject>> uiObjectList; // シーン内のゲームオブジェクトリスト
    std::queue<int> availableIDs;//消されたオブジェクトのIDのキューFIFO
    //std::unordered_map<std::string, std::vector<int>> nameToIDs; // オブジェクト名が同じオブジェクトのIDを検索
    std::vector<int> addObjects;//追加予定のオブジェクトリスト
    std::vector<int> removeObjects;//消去予定のオブジェクトリスト
    TextureManager& textureManager = TextureManager::GetInstance();
    SaveLoad saveload;
    Camera camera; // シーンごとのカメラ
    Sound& sound = Sound::GetInstance();
    DirectX::XMMATRIX vm;
    DirectX::XMMATRIX pm;
	//読み込むファイル名
	std::string sceneName;
};


