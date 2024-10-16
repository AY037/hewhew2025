#pragma once
#include "GameObject.h"
class Bullet :public GameObject
{
public:
	Bullet(TextureManager& _textureManager) {
		Initialize(L"asset/char01.png",_textureManager, 3, 4); //プレイヤーを初期化
		SetPos(0.0f, 0.0f, 0.0f);      //位置を設定
		SetSize(10.0f, 10.0f, 0.0f);	 //大きさを設定
		SetAngle(0.0f);                  //角度を設定
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	}
	~Bullet() { Uninit(); }
	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	Input input;
};

