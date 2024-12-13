#pragma once
#include "DynamicAABBTree.h"
#include "Component.h"

class BoxCollider :public Component
{
public:
	BoxCollider() {
	}
	~BoxCollider() {}
	void Init(GameObject& obj)override;
	void HitCheck(GameObject& obj);

	void Update()override;

private:
	DynamicAABBTree& tree = DynamicAABBTree::GetInstance();
};