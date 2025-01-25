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
	int dragCharging = 0;
	bool dragSwordHit = false;//引きずり剣が当たっているか
	DirectX::XMFLOAT3 dragSwordPos = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);//引きずり剣の座標
	int score = 0;//スコア
	DirectX::XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };
private:
	DirectX::XMFLOAT3 playerPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	int playerHP = 0;
};

