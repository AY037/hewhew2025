#pragma once
#include "Component.h"
class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody() {}
	void Init(GameObject& obj)override;
	void Update()override;
	void ResolveCollision(GameObject& other);
	void StaticCollision(GameObject& obj, GameObject& other);
	void DynamicCollision(GameObject& obj, GameObject& other);
	void ResolveOverlapStatic(GameObject& obj1, GameObject& obj2, float buffer = 0.0f);
	void ResolveOverlapDynamic(GameObject& obj1, GameObject& obj2,float buffer=0.0f);
private:
	const DirectX::XMFLOAT3 gravity = { 0.0f , -2.0f / 60.0f, 0.0f };
	const float airResistance = 0.03f;
	const float coefficient = 0.5f; // îΩî≠åWêîÇOÅ`ÇP
	int hit_objID=-1;
	bool update_flg = false;
};

