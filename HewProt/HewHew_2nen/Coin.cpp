#include "Coin.h"
#include "EventManager.h"

Coin::Coin()
{
	SetSize(10, 10, 0);
	AddComponent("BoxCollider");
}

Coin::~Coin()
{

}

void Coin::Init()
{
	Initialize("asset/Coin.png", 8, 1);
	m_BoxCollider = GetComponent<BoxCollider>("BoxCollider");
}

void Coin::Update()
{
	if (m_Flame_cnt == 4)
	{
		numU++;
		m_Flame_cnt = 0;
	}
	if (numU == 7)
	{
		numU = 0;
	}
	m_Flame_cnt++;

	auto hitObjects = m_BoxCollider->HitObjectName(*this);

	for (auto& pair : hitObjects)
	{
		if (pair.first == "Player")
		{
			EventManager::GetInstance().SendObjIdEvent("Delete",GetObjID());
		}
	}
}

