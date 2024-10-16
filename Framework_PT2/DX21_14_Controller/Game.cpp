#include "Game.h"
void Game::Init(HWND hWnd)
{
	D3D_Create(hWnd);//DirectX��������
	sceneManager = new SceneManager;
	sceneManager->AddScene();//�V�[���̒ǉ�
	sceneManager->SwitchScene(0);
}

void Game::Update(void)
{
	if (sceneManager != nullptr)
	{
		input.Update();
		sceneManager->Update();
		if (input.GetKeyTrigger(VK_SPACE))
		{
			sceneManager->SwitchScene(1);
		}
		if (input.GetKeyTrigger(VK_G))
		{
			delete sceneManager;
			sceneManager = nullptr;
		}
	}
}

void Game::Draw(void)
{
	D3D_StartRender();  //�`��J�n
	if (sceneManager != nullptr)
	{
		sceneManager->Draw();
	}
	D3D_FinishRender(); //�`��I��
}

void Game::Uninit(void)
{
	delete sceneManager;
	sceneManager = nullptr;
	D3D_Release();//DirectX���I��
}
