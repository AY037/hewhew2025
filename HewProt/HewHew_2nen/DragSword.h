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
	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};
