#include "Bullet.h"
#include "GameManager.h"
#include "EventManager.h"

void Bullet::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	SetSize(5, 10, 0);
	
	SetIsBoxColl(false);//汎用コライダーを利用しない

	DirectX::XMFLOAT3 _pos = GetPos();
	const DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();

	float angle_rad = std::atan2(_playerPos.y - _pos.y, _playerPos.x - _pos.x);
	shoot_angle = angle_rad * (180.0f / DirectX::XM_PI)-90;
	SetAngle(shoot_angle);
}

void Bullet::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	_velocity.x = -sin(DirectX::XMConvertToRadians(shoot_angle)) * speed;
	_velocity.y = cos(DirectX::XMConvertToRadians(shoot_angle)) * speed;;
	_velocity.x += 1.3f;//スクロール速度ぶん加算


	if (boxcoll.HitCheck(*this, "Player"))
	{
 		EventManager::GetInstance().SendObjIdEvent("Delete",GetObjID());
		EventManager::GetInstance().SendEvent("damage");
	}
	if (!flip)
	{
		if (boxcoll.HitCheck(*this, "Sword"))
		{
			shoot_angle += 180;
			SetAngle(shoot_angle);
			flip = true;
		}
	}

	if(flip)
	{
		int enemyId=-1;
		if (boxcoll.HitCheck(*this, "Enemy", &enemyId))
		{
			EventManager::GetInstance().SendObjIdEvent("Delete", GetObjID());
			if (enemyId != -1)
			{
				EventManager::GetInstance().SendObjIdEvent("TransDebri", enemyId);
			}
		}
	}

	//プレイヤーの速度の更新
	SetVelocity(_velocity);
	_pos.x += _velocity.x;
	_pos.y += _velocity.y;
	SetPos(_pos.x, _pos.y, 0);
}

void Bullet::Draw(void)
{
	//Render();      //プレイヤーを描画
}