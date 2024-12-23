#pragma once
#include <vector>
#include "GameObject.h"
#include "Sound.h"
#include "Camera.h"
#include <vector>
#include <memory>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <list>

class Scene {
public:
    Scene() : camera(nullptr) {} // cameraの初期化
    virtual ~Scene() {
    }

    virtual void Init() = 0;   // シーンの初期化。ここにオブジェクトを追加
    virtual void Update() = 0;  // シーン内のオブジェクト更新。
    virtual void Draw() = 0;    // シーン内のオブジェクトを描画
    virtual void Shutdown() {}    // シーンの終了処理。

protected:
    std::unique_ptr<Camera> camera; // シーンごとのカメラ
    Sound& sound = Sound::GetInstance();
};


