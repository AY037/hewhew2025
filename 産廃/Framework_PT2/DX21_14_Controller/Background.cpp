#include "Background.h"
void Background::Init(TextureManager& _textureManager)
{
	Initialize(L"asset/back_img_01.png", _textureManager);   //�w�i��������
	SetPos(0.0f, 0.0f, 0.0f);      //�ʒu��ݒ�
	SetSize(100.0f, 100.0f, 0.0f);  //�傫����ݒ�
	SetAngle(0.0f);                //�p�x��ݒ�
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
}

void Background::Update(void)
{
	
}

void Background::Draw(void)
{
	
}