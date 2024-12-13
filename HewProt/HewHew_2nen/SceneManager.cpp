#include "SceneManager.h"
#include "EventManager.h"
void SceneManager::Init()
{
	EventManager::GetInstance().SetChangeSceneFunc([this](const std::string sceneName) {this->SwitchScene(sceneName); });
}
void SceneManager::AddScene(const std::string& sceneName, Scene* scene) {
	scenes[sceneName]=std::move(scene);  // ���L�����ړ����Ēǉ�
}

void SceneManager::SwitchScene(const std::string& sceneName)//�V�[���̐؂�ւ��B
{
	if (scenes.find(sceneName) != scenes.end()) {
		// sceneName �Ɋ֘A����V�[���̃x�N�^�[���擾
		// �V�[������łȂ����Ƃ��m�F
		if (scenes[sceneName]) {
			// �x�N�^�[�̍ŏ��̃V�[�����擾
			currentScene = scenes[sceneName]; // ���L�����ړ�
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

void SceneManager::EngineUpdate()
{
	if (currentScene != nullptr)
	{
		currentScene->EngineUpdate(); // �A�N�e�B�u�ȃV�[���̍X�V
	}
}

void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(); // �A�N�e�B�u�ȃV�[���̕`��
	}
}