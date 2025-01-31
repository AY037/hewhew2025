#pragma once
#include "GameObject.h"
class BigAttackAnimation0 :public GameObject
{
public:
	BigAttackAnimation0(const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation");
		SetPos(_pos.x, _pos.y, 0);
		SetSize(30, 50, 0);
		SetAngle(20);
		SetObjectTexName("asset/bigAttack3.png");
		Init();
		isRigidbody = false;
	}
	~BigAttackAnimation0() {}

	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	int animation_cnt = 0;
};

