#include "SceneManager.h"
void SceneManager::AddScene(const std::string& sceneName, Scene* scene) {
	scenes[sceneName].push_back(std::move(scene));  // 所有権を移動して追加
}

void SceneManager::SwitchScene(const std::string& sceneName)//シーンの切り替え。
{
	if (scenes.find(sceneName) != scenes.end()) {
		// sceneName に関連するシーンのベクターを取得
		auto& sceneVector = scenes[sceneName];

		// シーンが空でないことを確認
		if (!sceneVector.empty()) {
			// ベクターの最初のシーンを取得
			currentScene = scenes[sceneName][0]; // 所有権を移動
		}
	}
}

void SceneManager::Update()// 現在のシーンの更新
{
	if (currentScene != nullptr)
	{
		currentScene->Update(); // アクティブなシーンの更新
	}
}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(); // アクティブなシーンの描画
	}
}