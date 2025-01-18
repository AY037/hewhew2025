#pragma once
#include "Component.h"
class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody() {}
	void Init(GameObject& obj)override;
	void Update()override;
	void ResolveCollision(GameObject& other, DirectX::XMFLOAT2& normal);
	void StaticCollision(GameObject& obj, GameObject& other, DirectX::XMFLOAT2& _normal);
	void DynamicCollision(GameObject& obj, GameObject& other);
private:
	const DirectX::XMFLOAT3 gravity = { 0.0f , -4.0f / 60.0f, 0.0f };
	const float airResistance = 0.03f;
	const float coefficient = 0.5f; // îΩî≠åWêîÇOÅ`ÇP
	int hit_objID=-1;
	bool update_flg = false;
};

