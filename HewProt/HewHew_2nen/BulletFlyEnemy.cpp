#include "BulletFlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"
#include "Sound.h"


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
				if (math::Max(std::fabs(GetVelocity().x), std::fabs(GetVelocity().y)) > 2.0f)
				{
					Sound::GetInstance().Play(SE_DESTROY);
				}
			}

			for (auto& pair : hitObject)
			{
				if (GetName() == "BulletFlyEnemy")
				{
					//剣に当たると残骸に
					if (pair.first == "Sword")
					{
						SetSize(5.0f, 5.0f, 0);
						SetObjTypeName("Debri");
						SetName("Debri");
						EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
						Sound::GetInstance().Play(SE_HIT);
					}
					//残骸が当たると
					if (pair.first == "Debri")
					{
						auto& obj = pair.second;
						DirectX::XMFLOAT3 velocity = obj->GetVelocity();
						float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
						if (velocityScale > 2.0f)
						{
							SetSize(5.0f, 5.0f, 0);
							SetObjTypeName("Debri");
							SetName("Debri");
							EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
							Sound::GetInstance().Play(SE_HIT);
						}
					}
					if (pair.first == "Player")
					{
						EventManager::GetInstance().SendEvent("damage");
					}
				}
				else
				{
					if (pair.first == "Enemy")
					{
						DirectX::XMFLOAT3 velocity = GetVelocity();
						float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
						if (velocityScale > 2.0f)
						{
							auto& obj = pair.second;
							obj->SetSize(5.0f, 5.0f, 0);
							obj->SetObjTypeName("Debri");
							obj->SetName("Debri");
							EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
							Sound::GetInstance().Play(SE_HIT);

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