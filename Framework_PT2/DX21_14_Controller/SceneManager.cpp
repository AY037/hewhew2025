#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
void SceneManager::AddScene()//シーンの追加。
{
	scenes.push_back(std::make_unique<TitleScene>());
	scenes.push_back(std::make_unique<GameScene>());
}

void SceneManager::SwitchScene(int sceneID)//シーンの切り替え。
{
	if (sceneID >= 0 && sceneID < scenes.size())
	{
		currentScene = std::move(scenes[sceneID]);
		currentScene->Init(); // シーンの初期化を行う
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