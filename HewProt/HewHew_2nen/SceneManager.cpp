#include "SceneManager.h"
#include "EventManager.h"
void SceneManager::Init()
{
	EventManager::GetInstance().SetChangeSceneFunc([this](const std::string sceneName) {this->SwitchScene(sceneName); });
}
void SceneManager::AddScene(const std::string& sceneName, Scene* scene) {
	scenes[sceneName]=std::move(scene);  // 所有権を移動して追加
}

void SceneManager::SwitchScene(const std::string& sceneName)//シーンの切り替え。
{
	if (scenes.find(sceneName) != scenes.end()) {
		// sceneName に関連するシーンのベクターを取得
		// シーンが空でないことを確認
		if (scenes[sceneName]) {
			// ベクターの最初のシーンを取得
			currentScene = scenes[sceneName]; // 所有権を移動
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

void SceneManager::EngineUpdate()
{
	if (currentScene != nullptr)
	{
		currentScene->EngineUpdate(); // アクティブなシーンの更新
	}
}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(); // アクティブなシーンの描画
	}
}