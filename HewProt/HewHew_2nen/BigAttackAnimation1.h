#pragma once
#include "GameObject.h"
class BigAttackAnimation1:public GameObject
{
public:
	BigAttackAnimation1(TextureManager& _textureManager, const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation");
		SetPos(_pos.x, _pos.y, 0);
		SetSize(60, 60, 0);
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
	int animation_cnt = 0;
};

