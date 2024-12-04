#include "Bullet.h"
#include <cstdio>
#include "iostream"
void Bullet::Init()
{
	// �R���\�[�������蓖�Ă�
	/*AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);*/
}
//�e�X�g
void Bullet::Update(void)
{
	DirectX::XMFLOAT3 direction;
	float pitch = shootAngle.y;
	float yaw = shootAngle.x+90;
	// �e�̕������v�Z (yaw: �����p�x, pitch: �����p�x)
	direction.x = cos(DirectX::XMConvertToRadians(pitch)) * cos(DirectX::XMConvertToRadians(yaw));
	direction.y = sin(DirectX::XMConvertToRadians(pitch));
	direction.z = cos(DirectX::XMConvertToRadians(pitch)) * sin(DirectX::XMConvertToRadians(yaw));

	DirectX::XMFLOAT3 pos=GetPos();
	// ���݂̈ʒu�Ɉړ��ʂ����Z
	pos.x -= direction.x * speed;
	pos.y -= direction.y * speed;
	pos.z += direction.z * speed;

	// �e�̐V�����ʒu��ݒ�
	SetPos(pos.x, pos.y, pos.z);
}

void Bullet::Draw(void)
{
	//Render();      //�v���C���[��`��
}