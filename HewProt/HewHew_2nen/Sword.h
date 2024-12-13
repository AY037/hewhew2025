#pragma once
#include "GameObject.h"
class Sword:public GameObject
{
public:
	Sword(const DirectX::XMFLOAT3& _swordSpeed):swordSpeed(_swordSpeed){
		AddComponent("BoxCollider");
		isRigidbody = true;
	}
	~Sword() {}
	
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
	float GetMass() override
	{
		return 100000.0f;
	}
private:
	DirectX::XMFLOAT3 swordSpeed;
};