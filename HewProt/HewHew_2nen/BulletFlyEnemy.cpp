#include "BulletFlyEnemy.h"
#include "GameManager.h"
#include "EventManager.h"

void BulletFlyEnemy::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager); //�v���C���[��������
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
			_velocity.x = -sin(DirectX::XMConvertToRadians(move_angle))*0.8 ;//���������̑��x�ɂ���
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle))*0.8;;
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

		if (firecnt == 1&&shoot_cnt>60)//�ʂ���񌂂�����
		{

			DirectX::XMFLOAT3 _playerPos = GameManager::GetInstance().GetPlayerPos();//�v���C���[�̃|�W�V�����擾
			float angle_rad = std::atan2(_playerPos.y + _pos.y, _playerPos.x +80 - _pos.x);//�v���C���[�̏����E�B���̓G�̍����ɂɈ��ڂ͈ړ�
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
			_velocity.y = cos(DirectX::XMConvertToRadians(move_angle))* -1.8;
		}

		if (firecnt == 3&&shoot_cnt>100)//3�񌂂����Ƃ���
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
		if (boxColl.HitCheck(*this, "Player"))
		{
		
			SetIsBoxColl(true);
			hit_attack = true;
		}

	}
	//--------------------------------------------------
	else
	{
		//�c�[�ɂȂ�����Rigidbody���I��
		components["Rigidbody"]->Update();
		components["BoxCollider"]->Update();
	}
}

void BulletFlyEnemy::Draw(void)
{
	//Render();      //�v���C���[��`��
}