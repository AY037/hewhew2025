#include "Game.h"
#include "GameScene.h"
#include "TitleScene.h"
void Game::Init(HWND hWnd)
{
	Sound::GetInstance().Init();
	sceneManager = new SceneManager;
	sceneManager->AddScene("TitleScene", new TitleScene);//シーンの追加
	sceneManager->AddScene("GameScene", new GameScene);//シーンの追加
	sceneManager->SwitchScene("GameScene");
}

void Game::Update(void)
{
	input.Update();
	if (sceneManager != nullptr)
	{
		sceneManager->Update();
	}
}

void Game::Draw(void)
{
	if (sceneManager != nullptr)
	{
		sceneManager->Draw();
	}
}

void Game::Uninit(void)
{
	delete sceneManager;
	sceneManager = nullptr;
	Sound::GetInstance().Uninit();
}
