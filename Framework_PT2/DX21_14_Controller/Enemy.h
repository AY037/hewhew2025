#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
public:
	Enemy(TextureManager& _textureManager, DirectX::XMFLOAT3 enemyPos) {
		Initialize(L"asset/char01.png", _textureManager, 3, 4); //�v���C���[��������
		SetPos(enemyPos.x, enemyPos.y, enemyPos.z);      //�ʒu��ݒ�
		SetSize(20.0f, 20.0f, 0.0f);	 //�傫����ݒ�
		SetAngle(0.0f);                  //�p�x��ݒ�
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	}
	~Enemy() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

