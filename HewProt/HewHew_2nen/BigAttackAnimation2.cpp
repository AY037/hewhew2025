#include "BigAttackAnimation2.h"

void BigAttackAnimation2::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 8, 1); //�v���C���[��������
	SetColor(0.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	numU = 0;
}

void BigAttackAnimation2::Update(void)
{
	static int animation_cnt = 0;
	animation_cnt++;
	if (animation_cnt == 3)
	{
		numU++;
		animation_cnt = 0;
	}
	if (numU >= 8) {
	}
}

void BigAttackAnimation2::Draw(void)
{
	//Render();      //�v���C���[��`��
}
