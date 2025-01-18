#include "Explosion.h"
#include "EventManager.h"

Explosion::Explosion()
{
	SetSize(30, 30, 0);
}

Explosion::~Explosion()
{

}

void Explosion::Init()
{
	Initialize("asset/fireParticle.png",4,4);
}

void Explosion::Update()
{
	if(m_Flame_cnt==2)
	{
		numU++;
		m_Flame_cnt=0;
	}
	if (numU == 3)
	{
		numU = 0;
		if (numV == 3)
		{
			EventManager::GetInstance().SendObjIdEvent("Delete", GetObjID());
		}
		else
		{
			numV++;
		}

	}
	m_Flame_cnt++;
}
