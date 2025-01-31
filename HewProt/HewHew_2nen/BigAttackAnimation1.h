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

	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	int animation_cnt = 0;
};

