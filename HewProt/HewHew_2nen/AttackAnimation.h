#pragma once
#include "GameObject.h"
#include "EventManager.h"
class AttackAnimation1 :public GameObject
{
public:
	AttackAnimation1(DirectX::XMFLOAT3& _pos):playerPos(_pos) {
		SetName("AttackAnimation");
		SetSize(70, 70, 0);
		SetAngle(-45);
		SetObjectTexName("asset/Attack.png");
		Init();
		isRigidbody = false;
	}
	~AttackAnimation1() {
	}

	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	DirectX::XMFLOAT3& playerPos;
};

