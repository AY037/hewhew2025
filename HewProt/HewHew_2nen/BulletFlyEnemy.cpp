#include "BulletFlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"
#include "Sound.h"


void BulletFlyEnemy::Init()
{
	Initialize(GetObjectTexName()); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
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

	//������낵���I�I
	//--------------------------------------------------------------------------
	//"�c�[�ɂȂ��Ă��Ȃ�������
	if (GetObjTypeName() != "Debri")
	{
		//shoot_time�t���[�����Ƃ�Bullet�𐶐�
		shoot_cnt++;
		if (shoot_cnt == shoot_time)
		{

			//��ID�𑗂��Ď����W��Bullet�𐶐�
			if (firecnt < 3)
			{
				EventManager::GetInstance().SendObjIdEvent("Shoot", GetObjID());
				firecnt += 1;//�e�����Ƒ��₷�B3�񌂂ƃA�^�b�N����

			}

			shoot_cnt = 0;
		}


		if (attack_fg == true)//true��������
		{
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//���������̑��x�ɂ���
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 0.8;;
		}
		else
		{
			_velocity.x = 1.0f;//�ړ����x�X�N���[�����x���኱�x�����炢
			_velocity.y = 0;
		}

		if (hit_attack == true)//�ːi�U��������������
		{
			_velocity.x = -2.0f;//��ʊO�Ɉړ�
		}

		if (firecnt == 1 && shoot_cnt > 60)//�ʂ���񌂂�����
		{

			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//�v���C���[�̃|�W�V�����擾
			float angle_rad = std::atan2(_playerPos.y + _pos.y, _playerPos.x + 80 - _pos.x);//�v���C���[�̏����E�B���̓G�̍����ɂɈ��ڂ͈ړ�
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//���������̑��x�ɂ���
			//_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 1.5;

		}

		if (firecnt == 2 && shoot_cnt > 60)//�ʂ�2�񌂂�����
		{
			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//�v���C���[�̃|�W�V�����擾
			float angle_rad = std::atan2(_playerPos.y + _pos.y, _playerPos.x + 300 - _pos.x);//�v���C���[�̏����E�B���̓G�̍����ɂɈ��ڂ͈ړ�
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 3.2;//���������̑��x�ɂ���
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * -1.8;
		}

		if (firecnt == 3 && shoot_cnt > 100)//3�񌂂����Ƃ���
		{
			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//�v���C���[�̃|�W�V�����擾
			float angle_rad = std::atan2(_playerPos.y - _pos.y, _playerPos.x + 50 - _pos.x);//�v���C���[�ɓˌ������邽�߂ɂ������X�����炵�Ă���
			move_angle = angle_rad * (180.0f / DirectX::XM_PI) - 90;
			//SetAngle(move_angle);
			attack_fg = true;//�U���t���O��true�ɂ���
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle)) * 0.8;//���������̑��x�ɂ���
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle)) * 0.8;;
		}

		SetVelocity(_velocity);

		auto boxCollider = GetComponent<BoxCollider>("BoxCollider");
		auto hitObject = boxCollider->HitObjectName(*this);

		if (GetName() == "Debri")
		{
			debriTime_cnt++;
		}

		//�ڐG�����I�u�W�F�N�g�������
		if (!hitObject.empty())
		{
			//�c�[�̈ړ����x��������ΐڐG����炷
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
					//���ɓ�����Ǝc�[��
					if (pair.first == "Sword")
					{
						SetSize(5.0f, 5.0f, 0);
						SetObjTypeName("Debri");
						SetName("Debri");
						EventManager::GetInstance().SendObjIdEvent("Explosion", GetObjID());
						Sound::GetInstance().Play(SE_HIT);
					}
					//�c�[���������
					if (pair.first == "Debri")
					{
						auto& obj = pair.second;
						DirectX::XMFLOAT3 velocity = obj->GetVelocity();
						float velocityScale = std::fabs(velocity.x) + std::fabs(velocity.y);
						//if (velocityScale > 2.0f)
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
		//�c�[�ɂȂ�����Rigidbody���I��
		for (auto& component : components)
		{
			component.second->Update();
		}
	}
}

void BulletFlyEnemy::Draw(void)
{
	//Render();      //�v���C���[��`��
}