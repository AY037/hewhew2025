#pragma once
#include "Scene.h"
/// <summary>
/// �Q�[�����̃V�[���i�X�e�[�W�A���j���[�Ȃǁj���Ǘ����܂��B
/// ���݂̃V�[���̐؂�ւ���A�V�[���̍X�V�E�I��������S�����܂��B
/// </summary>
class SceneManager
{
public:
	SceneManager() : currentScene(nullptr) {}
	~SceneManager() {
	}
	void AddScene();				   //�V�[���̒ǉ��B
	void SwitchScene(int sceneID);	   //�V�[���̐؂�ւ��B
	void Update();				       // ���݂̃V�[���̍X�V
	void Draw();                       // ���݂̃V�[���̕`��
private:
	std::vector<std::unique_ptr<Scene>> scenes; // ���ׂẴV�[���̃��X�g�B
	std::unique_ptr<Scene> currentScene;		// ���݃A�N�e�B�u�ȃV�[���B
};

