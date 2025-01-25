#pragma once
#include "Scene.h"
/// <summary>
/// �Q�[�����̃V�[���i�X�e�[�W�A���j���[�Ȃǁj���Ǘ����܂��B
/// ���݂̃V�[���̐؂�ւ���A�V�[���̍X�V�E�I��������S�����܂��B
/// </summary>
class SceneManager
{
public:
	static SceneManager& GetInstance() {
		static SceneManager sceneManager;
		return sceneManager;
	}

	void Init();
	void AddScene(const std::string& sceneName);	   //�V�[���̒ǉ��B
	void SwitchScene(const std::string& sceneName);	   //�V�[���̐؂�ւ��B
	void Update();				       // ���݂̃V�[���̍X�V
	void EngineUpdate();			   // ���݂̃V�[���̍X�V�i�G���W���g�p���j
	void Draw();                       // ���݂̃V�[���̕`��
private:
	SceneManager() { Init(); }
	~SceneManager() {
		scenes.clear(); // �}�b�v���̂��N���A
	}
	std::unordered_map<std::string,std::shared_ptr<Scene>> scenes; // ���ׂẴV�[���̃��X�g�B
	std::string currentSceneName;		// ���݃A�N�e�B�u�ȃV�[���B
};

