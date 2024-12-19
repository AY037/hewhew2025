#include "Enemy.h"
void Enemy::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void Enemy::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void Enemy::Draw(void)
{
	//Render();      //プレイヤーを描画
}
