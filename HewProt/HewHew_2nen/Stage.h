#pragma once
#include "GameObject.h"
class Stage:public GameObject
{
public:
	Stage() {
		AddComponent("StageCollider");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Stage.jpg");
	}
	~Stage() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

