#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class Enemy :public GameObject
{
public:
	Enemy() {
		AddComponent("Rigidbody");
		//AddComponent("BoxCollider");
		SetObjectTexName("asset/Enemy.png");
		SetObjTypeName("Enemy");
		isRigidbody = true;
		SetSize(16, 20, 0);
	}
	~Enemy() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	BoxCollider boxColl;
};

