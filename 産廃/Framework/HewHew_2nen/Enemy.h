#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{
public:
	Enemy() {
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		objectType = true;
	}
	~Enemy() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

