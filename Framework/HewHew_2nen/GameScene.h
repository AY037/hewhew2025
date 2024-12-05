#pragma once
#include "Scene.h"
#include"TextureManager.h"
#include "SaveLoad.h"
#include "Raycast.h"
#include "RayCollider.h"
#include <queue>
#include "Debug.h"
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

		// staticObjects�̉��
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
	void AddObject(std::shared_ptr<GameObject>_gameObject);
	void RenderGameObjectProperties(std::shared_ptr<GameObject>& obj);//�I�u�W�F�N�g��GUI�`��
	void RenderTextureSelector(std::shared_ptr<GameObject>& gameObject);//�I�u�W�F�N�g�̃e�N�X�`�����X�g��GUI
	void ControlGUI();
	//std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
	Input& input = Input::GetInstance();
	TextureManager textureManager;
	SaveLoad saveload;
	Debug& debug = Debug::GetInstance();
	std::queue<int> availableIDs;//�����ꂽ�I�u�W�F�N�g��ID�̃L���[FIFO


	std::string selected_ObjectName;//�I�����Ă���I�u�W�F�N�g�̖��O
	Raycast raycast;
	RayCollider collider;
	int selected_ObjectID = -1;//�I�����Ă���I�u�W�F�N�g��ID
	bool isUsingGUI = false;//�}�E�X�J�[�\����gui��ɂ��邩
	DirectX::XMMATRIX vm;
	DirectX::XMMATRIX pm;
};

