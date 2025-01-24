#include "BulletFlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"
#include "AudioManager.h"

void BulletFlyEnemy::Init()
{
	Initialize(GetObjectTexName()); //プレイヤーを初期化
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
			if (firecnt < 3)
			{
				EventManager::GetInstance().SendObjIdEvent("Shoot", GetObjID());
				firecnt += 1;//弾を撃つと増やす。3回撃つとアタックする

			}

			shoot_cnt = 0;
		}


		if (attack_fg == true)//trueだったら
		{
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//いい感じの速度にする
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 0.8;;
		}
		else
		{
			_velocity.x = 1.0f;//移動速度スクロール速度より若干遅いくらい
			_velocity.y = 0;
		}

		if (hit_attack == true)//突進攻撃が当たったら
		{
			_velocity.x = -2.0f;//画面外に移動
		}

		if (firecnt == 1 && shoot_cnt > 60)//玉を一回撃ったら
		{

			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//プレイヤーのポジション取得
			float angle_rad = std::atan2(_playerPos.y + _pos.y, _playerPos.x + 80 - _pos.x);//プレイヤーの少し右。その敵の高さにに一回目は移動
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//いい感じの速度にする
			//_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 1.5;

		}

		if (firecnt == 2 && shoot_cnt > 60)//玉を2回撃ったら
		{
			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//プレイヤーのポジション取得
			float angle_rad = std::atan2(_playerPos.y + _pos.y, _playerPos.x + 300 - _pos.x);//プレイヤーの少し右。その敵の高さにに一回目は移動
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 3.2;//いい感じの速度にする
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * -1.8;
		}

		if (firecnt == 3 && shoot_cnt > 100)//3回撃ったときに
		{
			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//プレイヤーのポジション取得
			float angle_rad = std::atan2(_playerPos.y - _pos.y, _playerPos.x + 50 - _pos.x);//プレイヤーに突撃させるためにちょっとXをずらしている
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			attack_fg = true;//攻撃フラグをtrueにする
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//いい感じの速度にする
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 0.8;;
		}

		SetVelocity(_velocity);

		auto boxCollider = GetComponent<BoxCollider>("BoxCollider");
		auto hitObject = boxCollider->HitObjectName(*this);

		if (GetName() == "Debri")
		{
			debriTime_cnt++;
		}

		//接触したオブジェクトがあれば
		if (!hitObject.empty())
		{
			//残骸の移動速度が早ければ接触音を鳴らす
			if (GetName() == "Debri")
			{
				auto velocity = GetVelocity();
				float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
				if (velocityScale > 2.5f)
				{
					AudioManager::GetInstance().PlayAudio(SE_DESTROY,false);
				}
			}

			for (auto& obj : hitObject)
			{
				std::string objTypeName = obj->GetObjTypeName();
				if (GetName() == "BulletFlyEnemy")
				{
					//剣に当たると残骸に
					if (objTypeName == "Sword")
					{
						EventManager::GetInstance().SendObjIdEvent("TransDebri", GetObjID());
						EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
						AudioManager::GetInstance().PlayAudio(SE_HIT, false);
					}
					//残骸が当たると
					if (objTypeName == "Debri")
					{
						DirectX::XMFLOAT3 velocity = obj->GetVelocity();
						float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
						if (velocityScale > 2.5f)
						{
							EventManager::GetInstance().SendObjIdEvent("TransDebri", GetObjID());
							EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
							AudioManager::GetInstance().PlayAudio(SE_HIT, false);
						}
					}
					if (objTypeName == "Player")
					{
						EventManager::GetInstance().SendEvent("damage");
					}
				}
				else
				{
					if (objTypeName == "Enemy")
					{
						DirectX::XMFLOAT3 velocity = GetVelocity();
						float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
						if (velocityScale > 2.5f)
						{
							EventManager::GetInstance().SendObjIdEvent("TransDebri", obj->GetObjID());
							EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
							AudioManager::GetInstance().PlayAudio(SE_HIT, false);

							if (debriTime_cnt > 120)
							{
								EventManager::GetInstance().SendObjIdEvent("Delete", GetObjID());
							}
						}
					}
				}
			}
		}
	}
	//--------------------------------------------------
	else
	{
		SetIsBoxColl(true);
		//残骸になったらRigidbodyをオン
		for (auto& component : components)
		{
			component.second->Update();
		}
	}
}

void BulletFlyEnemy::Draw(void)
{
	//Render();      //プレイヤーを描画
}