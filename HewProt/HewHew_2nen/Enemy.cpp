#include "Enemy.h"
#include "BoxCollider.h"
#include "EventManager.h"
#include "Sound.h"

void Enemy::Init()
{
	Initialize(GetObjectTexName()); //プレイヤーを初期化
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

	auto boxCollider = GetComponent<BoxCollider>("BoxCollider");
	auto hitObject = boxCollider->HitObjectName(*this);

	if (GetName() == "Debri")
	{
		debriTime_cnt++;
		SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);
	}
	else
	{
		SetPos(_pos.x + _velocity.x - 0.2f, _pos.y + _velocity.y, _pos.z + _velocity.z);
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
			if (GetName() == "Enemy")
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
					if(velocityScale>3.0f)
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
					if(velocityScale>2.0f)
					{
						auto& obj = pair.second;
						if (obj->GetName() == "BulletFlyEnemy");
						{
							obj->SetSize(5.0f, 5.0f, 0);
						}
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

	for (auto& component : components)
	{
		component.second->Update();
	}
}

void Enemy::Draw(void)
{
	//Render();      //プレイヤーを描画
}
