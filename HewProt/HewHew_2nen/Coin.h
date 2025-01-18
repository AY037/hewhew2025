#pragma once
#include "GameObject.h"
#include "BoxCollider.h"

class Coin :public GameObject
{
public:
	Coin();
	~Coin();

	void Init();
	void Update();
private:
	int m_Flame_cnt = 0;
	std::shared_ptr<BoxCollider> m_BoxCollider;
};

