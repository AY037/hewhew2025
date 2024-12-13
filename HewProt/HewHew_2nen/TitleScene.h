#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene()
	{
		camera = std::make_unique <Camera>();
		Init();
	}
	~TitleScene()
	{
		for (auto& pair : gameObjects) {
			pair.second->Uninit();
		}
		gameObjects.clear(); // �x�N�^���N���A

		sound.Stop(SOUND_LABEL_BGM000); //BGM���~
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
private:
};
