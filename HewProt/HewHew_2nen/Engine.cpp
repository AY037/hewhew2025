#include "Engine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "EventManager.h"
void Engine::Init(HWND hWnd)
{
	Sound::GetInstance().Init();
	sceneManager = new SceneManager;
	sceneManager->AddScene("TitleScene", new TitleScene);//シーンの追加
	sceneManager->AddScene("GameScene", new GameScene);//シーンの追加
	sceneManager->SwitchScene("GameScene");

	EventManager::GetInstance().AddListener("StartGame", [this]() {this->isRunning = true; });
	EventManager::GetInstance().AddListener("EndGame", [this]() {this->isRunning = false; });
}

void Engine::Update(void)
{
	input.Update();
	if (sceneManager != nullptr)
	{
		if(isRunning == true)
		{
			sceneManager->Update();
		}
		else
		{
			sceneManager->EngineUpdate();
		}
	}

#ifdef GUI_MODE
	guiController.Update();
#endif
}

void Engine::Draw(void)
{
	if (sceneManager != nullptr)
	{
		sceneManager->Draw();
	}
#ifdef GUI_MODE
	guiController.Draw();
#endif
}

void Engine::Uninit(void)
{
	delete sceneManager;
	sceneManager = nullptr;
	Sound::GetInstance().Uninit();
	Application::GetInstance().D3D_Release();//DirectXを終了
}