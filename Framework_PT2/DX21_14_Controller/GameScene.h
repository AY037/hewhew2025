#pragma once
#include "Scene.h"
#include"TextureManager.h"
/// <summary>
/// �Q�[���̈�̃V�[���i�X�e�[�W�A���j���[�A�퓬�Ȃǁj��\���N���X�B
/// �Q�[���I�u�W�F�N�g�̏W�����Ǘ����A���̃V�[���ł̑S�̗̂���𐧌䂵�܂��B
/// </summary>
class GameScene:public Scene
{
public:
	GameScene()
	{
		camera = new Camera();
	}
	~GameScene()
	{
		delete camera;
		for (auto obj : gameObjects)
		{
			obj->Uninit();
			if (obj) delete obj;
		}
		Shutdown();
		gameObjects.clear(); // �x�N�^���N���A
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
	void AddObject(GameObject*);
private:
	Input input;
	TextureManager textureManager;
};

