#include "AttackAnimation.h"

void AttackAnimation1::Init()
{
	Initialize(GetObjectTexName(), 10, 1); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	numU = 6;
}

void AttackAnimation1::Update(void)
{
	static int animation_cnt=0;
	animation_cnt++;
	if(animation_cnt==5)
	{
		numU++;
		animation_cnt = 0;
	}
	SetPos(playerPos.x, playerPos.y, 0);
}

void AttackAnimation1::Draw(void)
{
	//Render();      //�v���C���[��`��
}