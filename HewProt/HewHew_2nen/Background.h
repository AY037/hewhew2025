#pragma once
#include "GameObject.h"
class Background:public GameObject
{
public:
	Background(){
		//AddComponent("Rigidbody");
		//AddComponent("BoxCollider");
		SetObjectTexName("asset/background.png");
	}
	~Background() { }
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
};

