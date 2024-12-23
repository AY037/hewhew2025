#pragma once
#include "GameObject.h"
class FlyEnemy :public GameObject
{
public:
	FlyEnemy(){
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/FlyEnemy.png");//�����s����Ƃ�����ƐF�X�h�����ē��������邩��
		SetObjTypeName("Enemy");
		isRigidbody = true;
	}
	~FlyEnemy() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	const float speed = 1.0f;
};

