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
		isRunning = false;
		sceneManager = nullptr;
	}
	~Engine() {}
	SceneManager* sceneManager;// �V�[�����Ǘ����邽�߂̃I�u�W�F�N�g�B

	bool isRunning;// �Q�[�������s�����ǂ����𔻒肷��t���O�B

	Input& input = Input::GetInstance();

	GuiController guiController;
};
