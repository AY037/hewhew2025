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
	void Init();  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};

