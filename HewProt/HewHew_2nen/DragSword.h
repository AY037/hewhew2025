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
	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	BoxCollider boxCollider;
};
