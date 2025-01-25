#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
#include "Rigidbody.h"

class FallGround : public GameObject
{
public:
	FallGround();
	~FallGround();
	void Init();
	void Update();
private:
	BoxCollider boxColl;
	Rigidbody rigidbody;
	bool m_ActiveRigidbody = false;
	bool m_ActiveBoxcollider = false;
};