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
private:
	std::vector<std::shared_ptr<GameObject>> staticObjects; // �V�[�����̃Q�[���I�u�W�F�N�g���X�g
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>> dynamicObjects; // ���I�I�u�W�F�N�g�̊Ǘ�
	TextureManager textureManager;
};
