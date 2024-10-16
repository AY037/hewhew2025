#pragma once
#include "Scene.h"
/// <summary>
/// ゲーム内のシーン（ステージ、メニューなど）を管理します。
/// 現在のシーンの切り替えや、シーンの更新・終了処理を担当します。
/// </summary>
class SceneManager
{
public:
	SceneManager() : currentScene(nullptr) {}
	~SceneManager() {
	}
	void AddScene();				   //シーンの追加。
	void SwitchScene(int sceneID);	   //シーンの切り替え。
	void Update();				       // 現在のシーンの更新
	void Draw();                       // 現在のシーンの描画
private:
	std::vector<std::unique_ptr<Scene>> scenes; // すべてのシーンのリスト。
	std::unique_ptr<Scene> currentScene;		// 現在アクティブなシーン。
};

