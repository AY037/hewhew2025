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
		camera = std::make_unique<Camera>();
		Init();
	}
	~GameScene()
	{

		// staticObjects�̉��
		for (auto obj : staticObjects) {
			obj->Uninit();
		}
		staticObjects.clear(); // �x�N�^���N���A

		// dynamicObjects�̉��
		for (auto& pair : dynamicObjects) {
			for (auto& obj : pair.second) {
				obj->Uninit();
			}
		}
		dynamicObjects.clear(); // �x�N�^���N���A

		sound.Stop(SOUND_LABEL_BGM000); //BGM���~
	}
	void Init();	 // �V�[���̏������B�����ɃI�u�W�F�N�g��ǉ�
	void Update();	 // �V�[�����̃I�u�W�F�N�g�X�V�B
	void Draw();	 // �V�[�����̃I�u�W�F�N�g��`��
	void Shutdown(); // �V�[���̏I�������B
	void AddObject(const std::string& objectName, std::shared_ptr<GameObject> _gameObject);
	//void collider(GameObject&);//��ŃR���|�[�l���g��
	std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	std::vector<std::shared_ptr<GameObject>> staticObjects; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>> dynamicObjects; // ���I�I�u�W�F�N�g�̊Ǘ�
	Input& input = Input::GetInstance();
	TextureManager textureManager;
};

