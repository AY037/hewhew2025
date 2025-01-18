#pragma once
#include "GameObject.h"
#include "BoxCollider.h"

class DragSword :public GameObject
{
public:
	DragSword() {
		SetObjTypeName("DragSword");
		//SetObjectTexName("asset/block.png");
	}
	~DragSword() {}
	void Init();  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
	BoxCollider boxCollider;
};
