#pragma once
#include "GameObject.h"
class FlyEnemy :public GameObject
{
public:
	FlyEnemy(){
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/FlyEnemy.png");//を実行するとかすると色々派生して動きを作れるかも
		SetObjTypeName("Enemy");
		isRigidbody = true;
	}
	~FlyEnemy() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	const float speed = 1.0f;
};

