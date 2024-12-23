#include "DragAnimation.h"
void DragAnimation::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 5, 3); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�F��ݒ�
	numU = 0;
	numV = 0;
}

void DragAnimation::Update(void)
{
	DirectX::XMFLOAT3 size = GetSize();
	animation_cnt++;
	if (animation_cnt % 5==0)
	{
		numU++;
		//animation_cnt = 0;
	}
	if (numU == 5) {
		numU = 0;
	}

	if(size.x==30)
	{
		numV = 0;
	}
	if (size.x == 40|| size.x == 50)
	{
		numV = 2;
	}
}

void DragAnimation::Draw(void)
{
	//Render();      //�v���C���[��`��
}