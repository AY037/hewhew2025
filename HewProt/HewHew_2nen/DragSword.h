#pragma once
#include "GameObject.h"
class DragSword :public GameObject
{
public:
	DragSword() {
		AddComponent("BoxCollider");
		SetObjTypeName("DragSword");
	}
	~DragSword() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};
