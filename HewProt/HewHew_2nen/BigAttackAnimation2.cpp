#include "BigAttackAnimation2.h"
#include "EventManager.h"

void BigAttackAnimation2::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 4, 2); //プレイヤーを初期化
	SetColor(0.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	numU = 0;
	numV = 0;
}

void BigAttackAnimation2::Update(void)
{
	animation_cnt++;
	if (animation_cnt % 3 == 0)
	{
		numU++;
	}
	if (numU == 4 && numV == 0) {
		EventManager::GetInstance().SendEvent("deleteSword");
	}
	if (numU == 4) {
		numV = 1;
		numU = 0;
	}
	DirectX::XMFLOAT3 _pos = GetPos();

	SetPos(_pos.x + 5.0f, _pos.y + 1.6f, 0);
}

void BigAttackAnimation2::Draw(void)
{
	//Render();      //プレイヤーを描画
}
