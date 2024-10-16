#pragma once
#include "Scene.h"
class TitleScene:public Scene
{
public:
	TitleScene()
	{
		camera = new Camera();
	}
	~TitleScene()
	{
		Shutdown();
		delete camera;
		for (auto obj : gameObjects)
		{
			obj->Uninit();
			if (obj) delete obj;
		}
		gameObjects.clear(); // �x�N�^���N���A
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
private:
	TextureManager textureManager;
};
