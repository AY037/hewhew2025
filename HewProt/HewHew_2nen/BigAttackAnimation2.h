#pragma once
#pragma once
#include "GameObject.h"
class BigAttackAnimation2 :public GameObject
{
public:
	BigAttackAnimation2(const DirectX::XMFLOAT3 _pos) {
		SetName("BigAttackAnimation2");
		SetPos(_pos.x, _pos.y, 0);
		SetSize(80, 80, 0);
		SetAngle(20);
		SetObjectTexName("asset/bigAttack2.png");
		Init();
		isRigidbody = false;
	}
	~BigAttackAnimation2() {}

	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	int animation_cnt = 0;
};

