#include "SceneManager.h"
void SceneManager::AddScene(const std::string& sceneName, Scene* scene) {
	scenes[sceneName].push_back(std::move(scene));  // ���L�����ړ����Ēǉ�
}

void SceneManager::SwitchScene(const std::string& sceneName)//�V�[���̐؂�ւ��B
{
	if (scenes.find(sceneName) != scenes.end()) {
		// sceneName �Ɋ֘A����V�[���̃x�N�^�[���擾
		auto& sceneVector = scenes[sceneName];

		// �V�[������łȂ����Ƃ��m�F
		if (!sceneVector.empty()) {
			// �x�N�^�[�̍ŏ��̃V�[�����擾
			currentScene = scenes[sceneName][0]; // ���L�����ړ�
		}
	}
}

void SceneManager::Update()// ���݂̃V�[���̍X�V
{
	if (currentScene != nullptr)
	{
		currentScene->Update(); // �A�N�e�B�u�ȃV�[���̍X�V
	}
}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(); // �A�N�e�B�u�ȃV�[���̕`��
	}
}