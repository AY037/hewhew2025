#pragma once
#include "Scene.h"
class ResultScene :public Scene
{
public:
	ResultScene(const std::string& _sceneName)
	{
		sceneName = _sceneName;
	}
	~ResultScene()
	{

		sound.Stop(SOUND_LABEL_BGM000); //BGM���~
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
private:
};
