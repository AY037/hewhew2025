#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
#include "Rigidbody.h"

class DynamicStage : public GameObject
{
public:
	DynamicStage();
	~DynamicStage();
	void Init();
	void Update();
private:
	BoxCollider boxColl;
	Rigidbody rigidbody;
	bool m_ActiveRigidbody = false;
	bool m_ActiveBoxcollider = false;
};