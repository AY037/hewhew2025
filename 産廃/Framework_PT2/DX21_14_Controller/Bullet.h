#pragma once
#include "GameObject.h"
#include "Player.h"
class Bullet :public GameObject
{
public:
	Bullet(TextureManager& _textureManager,DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 playerAngle) {
		Init();
		Initialize(L"asset/char01.png",_textureManager, 3, 4); //�v���C���[��������
		SetPos(playerPos.x, playerPos.y, playerPos.z);      //�ʒu��ݒ�
		SetSize(10.0f, 10.0f, 0.0f);	 //�傫����ݒ�
		SetAngle(0.0f);                  //�p�x��ݒ�
		SetColor(1.0f, 1.0f, 1.0f, 0.5f);//�p�x��ݒ�
		shootAngle = playerAngle;
	}
	~Bullet() {  }
	void Init();  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	DirectX::XMFLOAT3 shootAngle;
	const float speed = 10.0f;
};

