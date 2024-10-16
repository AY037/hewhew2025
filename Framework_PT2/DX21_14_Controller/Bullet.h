#pragma once
#include "GameObject.h"
class Bullet :public GameObject
{
public:
	Bullet(TextureManager& _textureManager) {
		Initialize(L"asset/char01.png",_textureManager, 3, 4); //�v���C���[��������
		SetPos(0.0f, 0.0f, 0.0f);      //�ʒu��ݒ�
		SetSize(10.0f, 10.0f, 0.0f);	 //�傫����ݒ�
		SetAngle(0.0f);                  //�p�x��ݒ�
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	}
	~Bullet() { Uninit(); }
	void Init();  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	Input input;
};

