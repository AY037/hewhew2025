#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene(const std::string& _sceneName)
	{
		sceneName = _sceneName;
	}
	~TitleScene()
	{

	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
private:
};
