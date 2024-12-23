#include "GameManager.h"

GameManager::GameManager()
{

}

GameManager::~GameManager()
{

}

void GameManager::SetPlayerPos(const DirectX::XMFLOAT3& pPos)
{
	playerPos = pPos;
}

DirectX::XMFLOAT3 GameManager::GetPlayerPos()const
{
	return playerPos;
}

void GameManager::SetPlayerHP(const int pHP)
{
	playerHP = pHP;
}
int GameManager::GetPlayerHP() const
{
	return playerHP;
}