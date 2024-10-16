#pragma once
#include "Scene.h"
#include"TextureManager.h"
/// <summary>
/// ゲームの一つのシーン（ステージ、メニュー、戦闘など）を表すクラス。
/// ゲームオブジェクトの集合を管理し、そのシーンでの全体の流れを制御します。
/// </summary>
class GameScene:public Scene
{
public:
	GameScene()
	{
		camera = new Camera();
	}
	~GameScene()
	{
		delete camera;
		for (auto obj : gameObjects)
		{
			obj->Uninit();
			if (obj) delete obj;
		}
		Shutdown();
		gameObjects.clear(); // ベクタをクリア
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
	void AddObject(GameObject*);
private:
	Input input;
	TextureManager textureManager;
};

