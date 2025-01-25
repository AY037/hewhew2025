#include "BigAttackAnimation1.h"
#include "EventManager.h"

void BigAttackAnimation1::Init()
{
	Initialize(GetObjectTexName(), 4, 2); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�F��ݒ�
	numU = 0;
	numV = 0;
}

void BigAttackAnimation1::Update(void)
{
	DirectX::XMFLOAT3 size = GetSize();
	animation_cnt++;
	if (animation_cnt % 4==0)
	{
		numU++;
	}
	if (numU == 4) {
		numV = 1;
		numU = 0;
	}
	DirectX::XMFLOAT3 _pos = GetPos();

	SetPos(_pos.x + 4.0f, _pos.y + 1.0f, 0);
	SetSize(size.x, size.y * 1.02, 0);
}

void BigAttackAnimation1::Draw(void)
{
	//Render();      //�v���C���[��`��
}