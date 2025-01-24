#include "Goal.h"
#include "EventManager.h"

Goal::Goal()
{
	SetSize(30, 30, 0);
	AddComponent("BoxCollider");
}

Goal::~Goal()
{

}

void Goal::Init()
{
	Initialize("asset/Goal.png", 1, 1);
	m_BoxCollider = GetComponent<BoxCollider>("BoxCollider");
}

void Goal::Update()
{

	auto hitObjects = m_BoxCollider->HitObjectName(*this);

	for (auto& obj : hitObjects)
	{
		if (obj->GetObjTypeName() == "Player")
		{
			EventManager::GetInstance().SendChangeScene("ResultScene");
		}
	}
}