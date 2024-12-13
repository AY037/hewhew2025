#pragma once
#include "Scene.h"
/// <summary>
/// �Q�[�����̃V�[���i�X�e�[�W�A���j���[�Ȃǁj���Ǘ����܂��B
/// ���݂̃V�[���̐؂�ւ���A�V�[���̍X�V�E�I��������S�����܂��B
/// </summary>
class SceneManager
{
public:
	SceneManager() { Init(); }
	~SceneManager() {
		for (auto& scene : scenes)
		{
			delete scene.second;
		}
		scenes.clear(); // �}�b�v���̂��N���A
	}
	void Init();
	void AddScene(const std::string& sceneName, Scene*);				   //�V�[���̒ǉ��B
	void SwitchScene(const std::string& sceneName);	   //�V�[���̐؂�ւ��B
	void Update();				       // ���݂̃V�[���̍X�V
	void EngineUpdate();			   // ���݂̃V�[���̍X�V�i�G���W���g�p���j
	void Draw();                       // ���݂̃V�[���̕`��
private:
	std::unordered_map<std::string,Scene*> scenes; // ���ׂẴV�[���̃��X�g�B
	Scene* currentScene;		// ���݃A�N�e�B�u�ȃV�[���B
};

