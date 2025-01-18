#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class Enemy :public GameObject
{
public:
	Enemy() {
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Enemy.png");
		SetObjTypeName("Enemy");
		isRigidbody = true;
		SetSize(16, 20, 0);
	}
	~Enemy() {}
	void Init();  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	bool m_DebriFlg = false;//�c�[�ɂȂ�����
	int debriTime_cnt = 0;
};

