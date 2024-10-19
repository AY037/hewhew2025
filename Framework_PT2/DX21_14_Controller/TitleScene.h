#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene()
	{
		camera = std::make_unique <Camera>();
		Init();
	}
	~TitleScene()
	{
		// staticObjectsの解放
		for (auto obj : staticObjects) {
			obj->Uninit();
		}
		staticObjects.clear(); // ベクタをクリア

		// dynamicObjectsの解放
		for (auto& pair : dynamicObjects) {
			for (auto& obj : pair.second) {
				obj->Uninit();
			}
		}
		dynamicObjects.clear(); // ベクタをクリア

		sound.Stop(SOUND_LABEL_BGM000); //BGMを停止
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
private:
	std::vector<std::shared_ptr<GameObject>> staticObjects; // シーン内のゲームオブジェクトリスト
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>> dynamicObjects; // 動的オブジェクトの管理
	TextureManager textureManager;
};
