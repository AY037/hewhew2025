#pragma once
#include "SceneManager.h"
/// <summary>
/// ゲーム全体を管理するクラスです。
/// ゲームループの制御や、主要なゲームオブジェクトの初期化・更新・終了処理を行います。
/// </summary>
class Game{

private:
	SceneManager& sceneManager = SceneManager::GetInstance();// シーンを管理するための

	bool isRunning;// ゲームが実行中かどうかを判定するフラグ。

	Input& input = Input::GetInstance();
public:
	Game() {
		isRunning = false;
	}
	~Game(){}
	void Init(HWND hWnd); //初期化
	void Update(void);    //更新
	void Draw();          //描画
	void Uninit();        //終了
};