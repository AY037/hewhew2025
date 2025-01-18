#include "FlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"

void FlyEnemy::Init()
{
	Initialize(GetObjectTexName()); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	SetSize(20, 20, 0);
	for (const auto& pair : components)
	{
		pair.second->Init(*this);
	}
}

void FlyEnemy::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	_pos.x += _velocity.x;
	_pos.y += _velocity.y;
	SetPos(_pos.x, _pos.y, 0);

	if(GetObjTypeName()!="Debri")
	{
		const DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();

		float angle_rad = std::atan2(_playerPos.y - _pos.y, _playerPos.x - _pos.x);
		float angle = angle_rad * (180 / DirectX::XM_PI) - 90;
		//SetAngle(angle-90);

		_velocity.x = -sin(DirectX::XMConvertToRadians(angle)) * speed;
		_velocity.y = cos(DirectX::XMConvertToRadians(angle)) * speed;
		//プレイヤーの速度の更新
		SetVelocity(_velocity);
	}
	else
	{
		components["Rigidbody"]->Update();
	}

	components["BoxCollider"]->Update();
}

void FlyEnemy::Draw(void)
{
}