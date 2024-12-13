#pragma once
#pragma once
#include "GameObject.h"
class BigAttackAnimation2 :public GameObject
{
public:
	BigAttackAnimation2(TextureManager& _textureManager, const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation2");
		SetPos(_pos.x + 60, _pos.y + 20, 0);
		SetSize(80, 80, 0);
		SetAngle(30);
		SetObjectTexName("asset/bigAttack2.png");
		Init(_textureManager);
		isRigidbody = false;
	}
	~BigAttackAnimation2() {}

	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

