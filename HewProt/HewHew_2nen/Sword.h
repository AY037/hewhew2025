#pragma once
#include "GameObject.h"
class Sword:public GameObject
{
public:
	Sword(const DirectX::XMFLOAT3& _swordSpeed,const int _alive_time):swordSpeed(_swordSpeed),alive_time(_alive_time){
		AddComponent("BoxCollider");
		SetObjTypeName("Sword");
		isRigidbody = true;
	}
	~Sword() {}
	
	void Init(TextureManager& _textureManager);  //‰Šú‰»
	void Update();//XV
	void Draw();  //•`‰æ
	float GetMass() override
	{
		return 100000.0f;
	}
private:
	DirectX::XMFLOAT3 swordSpeed;
	int alive_time;
	int falme_cnt = 0;
};