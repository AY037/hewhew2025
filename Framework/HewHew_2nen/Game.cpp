#include "Game.h"
#include "GameScene.h"
#include "TitleScene.h"
void Game::Init(HWND hWnd)
{
	Sound::GetInstance().Init();
	sceneManager = new SceneManager;
	sceneManager->AddScene("TitleScene", new TitleScene);//�V�[���̒ǉ�
	sceneManager->AddScene("GameScene", new GameScene);//�V�[���̒ǉ�
	sceneManager->SwitchScene("GameScene");
}

void Game::Update(void)
{
	if (sceneManager != nullptr)
	{
		input.Update();
		sceneManager->Update();
		if (input.GetKeyTrigger(VK_G))
		{
			sceneManager->SwitchScene("GameScene");
		}
		if (input.GetKeyTrigger(VK_T))
		{
			//sceneManager->SwitchScene("TitleScene");
		}
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
	Application::GetInstance().D3D_Release();//DirectX���I��
}
