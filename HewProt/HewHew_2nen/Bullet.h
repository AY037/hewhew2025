#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class Bullet :public GameObject
{
public:
	Bullet() {
		//AddComponent("Rigidbody");�@�@      �n�ʂƂ��ђʂ��Ēǂ������邽�߂ɗ����I�t�ɂ��Ă���B���ۂɎg���Ƃ��̓R���|�[�l���g��ǂݍ����
		//AddComponent("BoxCollider");�@      �A�b�v�f�[�g���~�߂�B�v���C���[�Ɉ�x�ڐG�������Ƃ��m�F������t���O�ŃR���|�[�l���g�̃A�b�v�f�[�g
		SetObjectTexName("asset/Bullet.png");//�����s����Ƃ�����ƐF�X�h�����ē��������邩��
		SetObjTypeName("Bullet");
		//isRigidbody = true;
	}
	~Bullet() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	const float speed = 3.0f;
	BoxCollider boxcoll;
	float shoot_angle=0.0f;
	bool flip = false;//�e�̔��]�t���O
};

