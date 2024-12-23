#pragma once
#include "GameObject.h"
class Player :public GameObject
{
public:
	Player(){
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		objectType = true;
	}
	~Player() {}
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw();  //�`��
private:
	Input& input = Input::GetInstance();
};