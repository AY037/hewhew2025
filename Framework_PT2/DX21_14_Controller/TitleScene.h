#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene()
	{
		camera = new Camera();
	}
	~TitleScene()
	{
		Shutdown();
		delete camera;
		for (auto obj : gameObjects)
		{
			obj->Uninit();
			if (obj) delete obj;
		}
		gameObjects.clear(); // ベクタをクリア
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
private:
	TextureManager textureManager;
};
