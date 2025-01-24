#include "Enemy.h"
#include "BoxCollider.h"
#include "EventManager.h"
#include "AudioManager.h"

void Enemy::Init()
{
	Initialize(GetObjectTexName()); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
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

	//�ڐG�����I�u�W�F�N�g�������
	if (!hitObject.empty())
	{
		//�c�[�̈ړ����x��������ΐڐG����炷
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
				//���ɓ�����Ǝc�[��
				if (objTypeName == "Sword")
				{
					EventManager::GetInstance().SendObjIdEvent("TransDebri", GetObjID());
					EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
					AudioManager::GetInstance().PlayAudio(SE_HIT, false);
				}
				//�c�[���������
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
	//Render();      //�v���C���[��`��
}
