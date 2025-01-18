#pragma once
#include "GameObject.h"
#include "BoxCollider.h"

class Goal :public GameObject
{
public:
	Goal();
	~Goal();

	void Init();
	void Update();
private:
	std::shared_ptr<BoxCollider> m_BoxCollider;
};
