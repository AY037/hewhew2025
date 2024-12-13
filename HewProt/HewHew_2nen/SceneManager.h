#pragma once
#include "Scene.h"
/// <summary>
/// ゲーム内のシーン（ステージ、メニューなど）を管理します。
/// 現在のシーンの切り替えや、シーンの更新・終了処理を担当します。
/// </summary>
class SceneManager
{
public:
	SceneManager() { Init(); }
	~SceneManager() {
		for (auto& scene : scenes)
		{
			delete scene.second;
		}
		scenes.clear(); // マップ自体をクリア
	}
	void Init();
	void AddScene(const std::string& sceneName, Scene*);				   //シーンの追加。
	void SwitchScene(const std::string& sceneName);	   //シーンの切り替え。
	void Update();				       // 現在のシーンの更新
	void EngineUpdate();			   // 現在のシーンの更新（エンジン使用時）
	void Draw();                       // 現在のシーンの描画
private:
	std::unordered_map<std::string,Scene*> scenes; // すべてのシーンのリスト。
	Scene* currentScene;		// 現在アクティブなシーン。
};

