#pragma once
#include "GameObject.h"
class Player :public GameObject
{
public:
	Player(){
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		objectType = true;
	}
	~Player() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	Input& input = Input::GetInstance();
};