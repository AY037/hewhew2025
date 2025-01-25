#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
#include "Rigidbody.h"

class DynamicGround : public GameObject
{
public:
	DynamicGround();
	~DynamicGround();
	void Init();
	void Update();
private:
	BoxCollider boxColl;
	Rigidbody rigidbody;
};