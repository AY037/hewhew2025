#pragma once
#include "Scene.h"
/// <summary>
/// �Q�[�����̃V�[���i�X�e�[�W�A���j���[�Ȃǁj���Ǘ����܂��B
/// ���݂̃V�[���̐؂�ւ���A�V�[���̍X�V�E�I��������S�����܂��B
/// </summary>
class SceneManager
{
public:
	SceneManager() {}
	~SceneManager() {
		for (auto& pair : scenes) {
			for (auto* scene : pair.second) {
				delete scene; // Scene* �̃����������
			}
			pair.second.clear(); // �x�N�^���N���A
		}
		scenes.clear(); // �}�b�v���̂��N���A
	}
	void AddScene(const std::string& sceneName, Scene*);				   //�V�[���̒ǉ��B
	void SwitchScene(const std::string& sceneName);	   //�V�[���̐؂�ւ��B
	void Update();				       // ���݂̃V�[���̍X�V
	void Draw();                       // ���݂̃V�[���̕`��
private:
	std::unordered_map<std::string,std::vector<Scene*>> scenes; // ���ׂẴV�[���̃��X�g�B
	Scene* currentScene;		// ���݃A�N�e�B�u�ȃV�[���B
};

