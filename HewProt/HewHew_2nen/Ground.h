#pragma once
#include "GameObject.h"
class Ground :public GameObject
{
public:
	Ground() {
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Ground.jpg");
	}
	~Ground() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};