#pragma once
#include "Scene.h"
/// <summary>
/// ゲーム内のシーン（ステージ、メニューなど）を管理します。
/// 現在のシーンの切り替えや、シーンの更新・終了処理を担当します。
/// </summary>
class SceneManager
{
public:
	SceneManager() {}
	~SceneManager() {
		for (auto& pair : scenes) {
			for (auto* scene : pair.second) {
				delete scene; // Scene* のメモリを解放
			}
			pair.second.clear(); // ベクタをクリア
		}
		scenes.clear(); // マップ自体をクリア
	}
	void AddScene(const std::string& sceneName, Scene*);				   //シーンの追加。
	void SwitchScene(const std::string& sceneName);	   //シーンの切り替え。
	void Update();				       // 現在のシーンの更新
	void Draw();                       // 現在のシーンの描画
private:
	std::unordered_map<std::string,std::vector<Scene*>> scenes; // すべてのシーンのリスト。
	Scene* currentScene;		// 現在アクティブなシーン。
};

