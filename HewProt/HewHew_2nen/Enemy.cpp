#include "Enemy.h"
#include "BoxCollider.h"
#include "EventManager.h"
#include "AudioManager.h"

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
			auto velocity = GetVelocity();
			float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
			if (velocityScale > 2.5f)
			{
				AudioManager::GetInstance().PlayAudio(SE_DESTROY, false);
			}
		}

		for (auto& obj : hitObject)
		{
			std::string objTypeName = obj->GetObjTypeName();
			if (GetName() == "Enemy")
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
					if(velocityScale>2.5f)
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
					if(velocityScale>2.5f)
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

	for (auto& component : components)
	{
		component.second->Update();
	}
}

void Enemy::Draw(void)
{
	//Render();      //プレイヤーを描画
}
