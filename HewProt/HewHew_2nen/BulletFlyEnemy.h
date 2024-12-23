#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class BulletFlyEnemy :public GameObject
{
public:
	BulletFlyEnemy() {
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/FlyEnemy.png");
		SetName("BulletFlyEnemy");
		SetObjTypeName("Enemy");//�I�u�W�F�N�g�̑����G�l�~�[���ʂ̏����Ŏg���p��
		isRigidbody = true;//�ÓI�����I��
	}
	~BulletFlyEnemy() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	BoxCollider boxColl;
	int shoot_cnt = 0;//bullet�����p�t���[���J�E���g
	const int shoot_time = 120;//Bullet�����̃��[�g�Ⴍ����Α����Ȃ�
};

