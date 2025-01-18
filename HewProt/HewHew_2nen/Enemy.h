#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class Enemy :public GameObject
{
public:
	Enemy() {
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Enemy.png");
		SetObjTypeName("Enemy");
		isRigidbody = true;
		SetSize(16, 20, 0);
	}
	~Enemy() {}
	void Init();  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
	bool m_DebriFlg = false;//cŠ[‚É‚È‚Á‚½‚©
	int debriTime_cnt = 0;
};

