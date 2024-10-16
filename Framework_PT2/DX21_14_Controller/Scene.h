#pragma once
#include <vector>
#include "GameObject.h"
#include "Sound.h"
#include "Camera.h"
#include <vector>
#include <memory>
class Scene
{
public:
	Scene() {}
	virtual ~Scene() {
	}
	virtual void Init() = 0;	 // シーンの初期化。ここにオブジェクトを追加
	virtual void Update() = 0;	 // シーン内のオブジェクト更新。
	virtual void Draw() = 0;	 // シーン内のオブジェクトを描画
	virtual void Shutdown() = 0; // シーンの終了処理。
protected:
	std::vector<GameObject*> gameObjects; //シーン内のゲームオブジェクトリスト。
	Camera* camera;						  // シーンごとのカメラ
	Sound sound;
};

