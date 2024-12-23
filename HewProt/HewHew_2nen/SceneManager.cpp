#include "SceneManager.h"
#include "EventManager.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "GameScene.h"

void SceneManager::Init()
{
	EventManager::GetInstance().SetChangeSceneFunc([this](const std::string sceneName) {this->SwitchScene(sceneName); });

	EventManager::GetInstance().AddListener("TitleScene", [this]() { scenes["TitleScene"] = std::make_shared<TitleScene>("TitleScene"); });
	EventManager::GetInstance().AddListener("ResultScene", [this]() { scenes["ResultScene"] = std::make_shared<ResultScene>("ResultScene"); });
	EventManager::GetInstance().AddListener("Stage1", [this]() { scenes["Stage1"] = std::make_unique<GameScene>("Stage1"); });
	EventManager::GetInstance().AddListener("Stage2", [this]() { scenes["Stage2"] = std::make_unique<GameScene>("Stage2"); });
	EventManager::GetInstance().AddListener("Stage3", [this]() { scenes["Stage3"] = std::make_unique<GameScene>("Stage3"); });
	EventManager::GetInstance().AddListener("Stage4", [this]() { scenes["Stage4"] = std::make_unique<GameScene>("Stage4"); });
	EventManager::GetInstance().AddListener("Stage5", [this]() { scenes["Stage5"] = std::make_unique<GameScene>("Stage5"); });

}
void SceneManager::AddScene(const std::string& sceneName) {
	EventManager::GetInstance().SendEvent(sceneName);
}

void SceneManager::SwitchScene(const std::string& sceneName)//シーンの切り替え。
{
	if(scenes.size()!=0)
	{
		scenes[currentSceneName]->Shutdown();
	}
	AddScene(sceneName);
	currentSceneName = sceneName;
	scenes[currentSceneName]->Init();
}

void SceneManager::Update()// 現在のシーンの更新
{
	if (scenes[currentSceneName] != nullptr)
	{
		scenes[currentSceneName]->Update(); // アクティブなシーンの更新
	}
	std::string removeSceneName;
	for (auto& pair : scenes)
	{
		if (pair.first != currentSceneName)
		{
			removeSceneName = pair.first;
		}
	}
	scenes.erase(removeSceneName);
}

void SceneManager::EngineUpdate()
{
	if (scenes[currentSceneName] != nullptr)
	{
		scenes[currentSceneName]->EngineUpdate(); // アクティブなシーンの更新
	}
}

void SceneManager::Draw()
{
	if (scenes[currentSceneName] != nullptr)
	{
		scenes[currentSceneName]->Draw(); // アクティブなシーンの描画
	}
}