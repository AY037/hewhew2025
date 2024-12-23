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
			EventManager::GetInstance().SendObjIdEvent("Shoot", GetObjID());
			shoot_cnt = 0;
		}
		_velocity.x = 1.0f;//�ړ����x�X�N���[�����x���኱�x�����炢
		SetVelocity(_velocity);
		components["BoxCollider"]->Update();
	}
	//--------------------------------------------------------------------------
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