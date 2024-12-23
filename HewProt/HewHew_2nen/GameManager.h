#pragma once
#include <DirectXMath.h>
//シングルトン
class GameManager
{
public:
	GameManager();
	~GameManager();
	static GameManager& GetInstance()
	{
		static GameManager instance;
		return instance;
	}
	//プレイヤー座標、エネミーのプレイヤー座標取得用
	void SetPlayerPos(const DirectX::XMFLOAT3& pPos);
	DirectX::XMFLOAT3 GetPlayerPos() const;
	//プレイヤーHP、残存HP描画用
	void SetPlayerHP(const int pHP);
	int GetPlayerHP() const;
private:
	DirectX::XMFLOAT3 playerPos;
	int playerHP = 0;
};

