#pragma once
#include "GameObject.h"
class BigAttackAnimation1:public GameObject
{
public:
	BigAttackAnimation1(const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation");
		SetPos(_pos.x, _pos.y, 0);
		SetSize(60, 60, 0);
		SetAngle(20);
		SetObjectTexName("asset/bigAttack.png");
		Init();
		isRigidbody = false;
	}
	~BigAttackAnimation1() {}

	void Init();  //èâä˙âª
	void Update();//çXêV
	void Draw();  //ï`âÊ
private:
	int animation_cnt = 0;
};

