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
	std::ifstream fin;//読み出し用ファイルストリーム
	//ファイル書き込み処理
	std::ofstream fout;//書き込み用ファイルストリーム
	int moneyBuffer;
	std::string writeBuffer;//ファイル書き込み用バッファ
	GameObjectManager& gom = GameObjectManager::GetInstance();
};
