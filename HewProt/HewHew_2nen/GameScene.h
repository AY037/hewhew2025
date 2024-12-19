#pragma once
#include "Scene.h"
#include "Raycast.h"
#include "RayCollider.h"
#include "Debug.h"
#include "GuiController.h"

#define NOSELECTED (-1)//�I�u�W�F�N�g��I�����Ă��Ȃ�
/// <summary>
/// �Q�[���̈�̃V�[���i�X�e�[�W�A���j���[�A�퓬�Ȃǁj��\���N���X�B
/// �Q�[���I�u�W�F�N�g�̏W�����Ǘ����A���̃V�[���ł̑S�̗̂���𐧌䂵�܂��B
/// </summary>
class GameScene :public Scene
{
public:
	GameScene()
	{
		camera = std::make_unique<Camera>();
		Init();
	}
	~GameScene()
	{

		for (auto& pair : gameObjects) {
			pair.second->Uninit();
		}
		gameObjects.clear(); // �x�N�^���N���A

		sound.Stop(SOUND_LABEL_BGM000); //BGM���~
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void AddEventManager();
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
	//std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	Input& input = Input::GetInstance();
	Debug& debug = Debug::GetInstance();

	//�a���̎���
	int slash_time = 0;

	//�q�b�g�X�g�b�v�p
	int hit_stop = 0;
	int stop_cnt = 0;

	int PLAYER_ID = 0;
	int DRAGSWORD_ID = 0;
};

