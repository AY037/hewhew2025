#pragma once
#include "GameObject.h"
#include "EventManager.h"
class AttackAnimation1 :public GameObject
{
public:
	AttackAnimation1(TextureManager& _textureManager, DirectX::XMFLOAT3& _pos):playerPos(_pos) {
		SetName("AttackAnimation");
		SetSize(70, 70, 0);
		SetAngle(-45);
		SetObjectTexName("asset/Attack.png");
		Init(_textureManager);
		isRigidbody = false;
	}
	~AttackAnimation1() {
	}

	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
	DirectX::XMFLOAT3& playerPos;
};

