#include "Engine.h"
#include "EventManager.h"
#include "AudioManager.h"

void Engine::Init(HWND hWnd)
{
	//サウンドの初期化
	AudioManager::GetInstance().Init();
	AudioManager::GetInstance().LoadSounds();
	sceneManager.SwitchScene("Stage1");

	EventManager::GetInstance().AddListener("StartGame", [this]() {
		this->isRunning = true; 

		});
	EventManager::GetInstance().AddListener("EndGame", [this]() {this->isRunning = false; });
}

void Engine::Update(void)
{
	input.Update();
	if (&sceneManager != nullptr)
	{
		if(isRunning == true)
		{
			sceneManager.Update();
		}
		else
		{
			sceneManager.EngineUpdate();
		}
	}

#ifdef GUI_MODE
	guiController.Update();
#endif
}

void Engine::Draw(void)
{
	if (&sceneManager != nullptr)
	{
		sceneManager.Draw();
	}
#ifdef GUI_MODE
	guiController.Draw();
#endif
}

void Engine::Uninit(void)
{
	AudioManager::GetInstance().UnInit();
	Application::GetInstance().D3D_Release();//DirectXを終了
}