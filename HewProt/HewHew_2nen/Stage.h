#pragma once
#include "GameObject.h"
class Stage:public GameObject
{
public:
	Stage() {
		AddComponent("StageCollider");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Stage.jpg");
	}
	~Stage() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};

