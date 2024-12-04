#include "Enemy.h"
void Enemy::Init(TextureManager& _textureManager)
{
	Initialize("asset/yuusya_game.png", _textureManager); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Enemy::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);
	for (auto& component : components)
	{
		component->Update();
	}
}

void Enemy::Draw(void)
{
	//Render();      //�v���C���[��`��
}
