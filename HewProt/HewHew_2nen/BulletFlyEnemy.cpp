#include "BulletFlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"

void BulletFlyEnemy::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	SetSize(20, 20, 0);
	for(const auto& pair: components)
	{
		pair.second->Init(*this);
	}
	SetIsBoxColl(false);
}

void BulletFlyEnemy::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	_pos.y += _velocity.y;
	_pos.x += _velocity.x;
	SetPos(_pos.x, _pos.y, 0);

	//ここよろしく！！
	//--------------------------------------------------------------------------
	//"残骸になっていなかったら
	if (GetObjTypeName() != "Debri")
	{
		//shoot_timeフレームごとにBulletを生成
		shoot_cnt++;
		if (shoot_cnt == shoot_time)
		{
			//自IDを送って自座標にBulletを生成
			EventManager::GetInstance().SendObjIdEvent("Shoot", GetObjID());
			shoot_cnt = 0;
		}
		_velocity.x = 1.0f;//移動速度スクロール速度より若干遅いくらい
		SetVelocity(_velocity);
		if (boxColl.HitCheck(*this, "Sword"))
		{
			EventManager::GetInstance().SendObjIdEvent("TransDebri",GetObjID());
			SetIsBoxColl(true);
		}
		if (boxColl.HitCheck(*this, "Debri"))
		{
			EventManager::GetInstance().SendObjIdEvent("TransDebri", GetObjID());
			SetIsBoxColl(true);
		}
	}
	//--------------------------------------------------------------------------
	else
	{
		//残骸になったらRigidbodyをオン
		components["Rigidbody"]->Update();
		components["BoxCollider"]->Update();
	}
}

void BulletFlyEnemy::Draw(void)
{
	//Render();      //プレイヤーを描画
}