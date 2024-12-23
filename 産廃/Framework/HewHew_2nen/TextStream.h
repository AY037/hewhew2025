#pragma once
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "GameObjectManager.h"
class TextStream
{
public:
	void SaveScene(std::string fileName, const std::unordered_map<int, std::shared_ptr<GameObject>>& _tiles);
	void LoadScene(std::string fileName, std::unordered_map<int, std::shared_ptr<GameObject>>& tiles);
	std::ifstream fin;//�ǂݏo���p�t�@�C���X�g���[��
	//�t�@�C���������ݏ���
	std::ofstream fout;//�������ݗp�t�@�C���X�g���[��
	int moneyBuffer;
	std::string writeBuffer;//�t�@�C���������ݗp�o�b�t�@
	GameObjectManager& gom = GameObjectManager::GetInstance();
};
