#include "AttackAnimation.h"

void AttackAnimation1::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 10, 1); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	numU = 4;
}

void AttackAnimation1::Update(void)
{
	static int animation_cnt=0;
	animation_cnt++;
	if(animation_cnt==5)
	{
		numU++;
		animation_cnt = 0;
	}
	if (numU == 8&& animation_cnt == 0) {
	}
	if (numU >= 10) {
	}
}

void AttackAnimation1::Draw(void)
{
	//Render();      //プレイヤーを描画
}