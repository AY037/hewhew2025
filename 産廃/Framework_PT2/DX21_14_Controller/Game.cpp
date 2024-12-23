#include "Game.h"
#include "GameScene.h"
#include "TitleScene.h"
void Game::Init(HWND hWnd)
{
	//D3D_Create(hWnd);//DirectXを初期化
	Sound::GetInstance().Init();
	sceneManager = new SceneManager;
	sceneManager->AddScene("TitleScene", new TitleScene);//シーンの追加
	sceneManager->AddScene("GameScene", new GameScene);//シーンの追加
	sceneManager->SwitchScene("TitleScene");
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
			sceneManager->SwitchScene("TitleScene");
		}
	}
}

void Game::Draw(void)
{
	D3D_StartRender();  //描画開始
	if (sceneManager != nullptr)
	{
		sceneManager->Draw();
	}
	D3D_FinishRender(); //描画終了
}

void Game::Uninit(void)
{
	delete sceneManager;
	sceneManager = nullptr;
	Sound::GetInstance().Uninit();
	D3D_Release();//DirectXを終了
}
