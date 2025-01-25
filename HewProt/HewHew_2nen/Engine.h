#pragma once
#include "SceneManager.h"
#include "GuiController.h"
#include <memory>
class Engine
{
public:
	static Engine& GetInstance()
	{
		static Engine engine;
		return engine;
	}
	void Init(HWND hWnd); //������
	void Update(void);    //�X�V
	void Draw();          //�`��
	void Uninit();        //�I��
	GuiController& GetGuiController()
	{
		return guiController;
	}
private:
	Engine() {
	}
	~Engine() {}
	SceneManager& sceneManager = SceneManager::GetInstance();// �V�[�����Ǘ����邽�߂�

	bool isRunning;// �Q�[�������s�����ǂ����𔻒肷��t���O�B

	Input& input = Input::GetInstance();

	GuiController guiController;
};

