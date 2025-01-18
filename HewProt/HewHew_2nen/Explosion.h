#pragma once
#include "GameObject.h"
class Explosion:public GameObject
{
public:
	Explosion();
	~Explosion();

	void Init();
	void Update();
private:
	int m_Flame_cnt = 0;
};

