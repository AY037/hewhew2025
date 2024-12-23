#include "Player.h"
#include "EventManager.h"
void Player::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(),  _textureManager, 3, 4); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Player::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();


	if (input.GetKeyPress(VK_SPACE))
	{
		//プレイヤーのアニメーション
		numU++;
		if (numU >= 3) {
			numU = 0;
		}

		_velocity.x = 2;
		SetVelocity(_velocity);
	}

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);

	for (auto& component : components)
	{
		component->Update();
	}
}

void Player::Draw(void)
{
	//Render();      //プレイヤーを描画
}