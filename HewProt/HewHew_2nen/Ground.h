#pragma once
#include "GameObject.h"
class Ground :public GameObject
{
public:
	Ground() {
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Ground.jpg");
		SetObjTypeName("Ground");
	}
	~Ground() {}
	void Init();  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};