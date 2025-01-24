#pragma once
#include "Scene.h"
#include "DrawNumbers.h"
class ResultScene :public Scene
{
public:
	ResultScene(const std::string& _sceneName)
	{
		sceneName = _sceneName;
	}
	~ResultScene()
	{

	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
private:
	DrawNumbers numbers;
};
