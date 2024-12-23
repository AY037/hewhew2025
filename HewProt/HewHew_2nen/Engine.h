#pragma once
#include "SceneManager.h"
#include "GuiController.h"
#include <memory>
class Engine
{
public:
	static Engine& GetInstance()
	{
		static Engine engine;
		return engine;
	}
	void Init(HWND hWnd); //初期化
	void Update(void);    //更新
	void Draw();          //描画
	void Uninit();        //終了
	GuiController& GetGuiController()
	{
		return guiController;
	}
private:
	Engine() {
	}
	~Engine() {}
	std::unique_ptr<SceneManager> sceneManager =std::make_unique<SceneManager>();// シーンを管理するためのオブジェクト。

	bool isRunning;// ゲームが実行中かどうかを判定するフラグ。

	Input& input = Input::GetInstance();

	GuiController guiController;
};

