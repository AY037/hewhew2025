#pragma once
#include "GameObject.h"
class Ground :public GameObject
{
public:
	Ground() {
		AddComponent("BoxCollider");
		SetObjectTexName("asset/Ground.jpg");
		SetObjTypeName("Ground");
	}
	~Ground() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
};