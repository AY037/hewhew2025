#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
public:
	Enemy(TextureManager& _textureManager, DirectX::XMFLOAT3 enemyPos) {
		Initialize(L"asset/char01.png", _textureManager, 3, 4); //プレイヤーを初期化
		SetPos(enemyPos.x, enemyPos.y, enemyPos.z);      //位置を設定
		SetSize(20.0f, 20.0f, 0.0f);	 //大きさを設定
		SetAngle(0.0f);                  //角度を設定
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	}
	~Enemy() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};

