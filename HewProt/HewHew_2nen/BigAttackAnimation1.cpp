#include "BigAttackAnimation1.h"

void BigAttackAnimation1::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 8, 1); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�F��ݒ�
	numU = 0;
}

void BigAttackAnimation1::Update(void)
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

void BigAttackAnimation1::Draw(void)
{
	//Render();      //�v���C���[��`��
}