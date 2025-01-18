#pragma once
#include "GameObject.h"
class Stage:public GameObject
{
public:
	Stage() {
		AddComponent("StageCollider");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Stage.jpg");
		SetObjTypeName("Stage");
	}
	~Stage() {}
	void Init();  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
private:
};

