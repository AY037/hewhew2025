#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene(const std::string& _sceneName)
	{
		sceneName = _sceneName;
	}
	~TitleScene()
	{

		sound.Stop(SOUND_LABEL_BGM000); //BGMを停止
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
private:
};
