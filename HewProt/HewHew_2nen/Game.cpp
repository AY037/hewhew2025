#include "Game.h"
#include "TitleScene.h"
void Game::Init(HWND hWnd)
{
	Sound::GetInstance().Init();
	sceneManager = new SceneManager;
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
