#pragma once
#include "GameObject.h"
class BigAttackAnimation1:public GameObject
{
public:
	BigAttackAnimation1(TextureManager& _textureManager, const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation");
		SetPos(_pos.x + 40, _pos.y + 10, 0);
		SetSize(50, 50, 0);
		SetAngle(30);
		SetObjectTexName("asset/bigAttack.png");
		Init(_textureManager);
		isRigidbody = false;
	}
	~BigAttackAnimation1() {}

	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

