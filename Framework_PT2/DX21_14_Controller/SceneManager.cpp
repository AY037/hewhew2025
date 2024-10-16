#include "SceneManager.h"
#include "GameScene.h"
#include "TitleScene.h"
void SceneManager::AddScene()//�V�[���̒ǉ��B
{
	scenes.push_back(std::make_unique<TitleScene>());
	scenes.push_back(std::make_unique<GameScene>());
}

void SceneManager::SwitchScene(int sceneID)//�V�[���̐؂�ւ��B
{
	if (sceneID >= 0 && sceneID < scenes.size())
	{
		currentScene = std::move(scenes[sceneID]);
		currentScene->Init(); // �V�[���̏��������s��
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