#include "Game.h"
#include "AudioManager.h"

void Game::Init(HWND hWnd)
{
	//サウンドの初期化
	AudioManager::GetInstance().Init();
	AudioManager::GetInstance().LoadSounds();
	sceneManager->SwitchScene("Stage1");
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
	sceneManager.reset();
	AudioManager::GetInstance().UnInit();
	Application::GetInstance().D3D_Release();//DirectXを終了
}
