#include "FallWall.h"
#include "BoxCollider.h"
#include "EventManager.h"


using namespace DirectX::SimpleMath;

FallWall::FallWall()
{
	AddComponent("BoxCollider");
	SetObjectTexName("asset/FallWall.jpg");
	SetObjTypeName("Ground");
}

FallWall::~FallWall()
{

}

void FallWall::Init()
{
	Initialize(GetObjectTexName());   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void FallWall::Update()
{
	auto boxCollider = GetComponent<BoxCollider>("BoxCollider");

	auto hitObjects = boxCollider->HitObjectName(*this);

	for (auto& pair : hitObjects)
	{
		if (pair.first == "Sword" && !m_FallFlg)
		{
			m_FallFlg = true;

			Vector3 pos = GetPos();
			Vector3 size = GetSize();
			//‰ñ“]‚ÌŽx“_‚Æ”¼Œa‚ðŒvŽZ
			m_FulcrumPos = Vector3(pos.x + size.x / 2, pos.y - size.y / 2, 0.0f);
			m_R = size.y / 2;

			EventManager::GetInstance().SendObjIdEvent("Delete", pair.second->GetObjID());
		}

		if (pair.first == "Ground" || pair.first == "Stage")
		{
			m_FallSpeed = 0.0f;

			float angle = GetAngle();
			if (angle >= 265.9f && angle <= 274.1f)
			{
				m_FallFlg = false;

			}
		}

		if (pair.first == "Enemy")
		{
			if(m_FallFlg)
			{
				int id = pair.second->GetObjID();
				EventManager::GetInstance().SendObjIdEvent("Explosion", id);
				EventManager::GetInstance().SendObjIdEvent("TransDebri", id);
			}
		}
	}

	//“|‚ê‚é
	if (m_FallFlg)
	{
		Vector3 newPos;
		float angle = GetAngle();

		//‰ñ“]‘¬“x‚Æ—Ž‰º‘¬“x‚ð™X‚É‘¬‚ß‚é
		m_RotateSpeed +=(m_RotateSpeed>4.0f)?0.0f: 0.1f;
		m_FallSpeed += 0.05f;

		angle-= m_RotateSpeed;
		m_FulcrumPos.x += 0.3f;
		m_FulcrumPos.y -= m_FallSpeed;

		newPos.x = -sin(DirectX::XMConvertToRadians(angle))*m_R;
		newPos.y = cos(DirectX::XMConvertToRadians(angle)) * m_R;

		newPos += m_FulcrumPos;

		SetAngle(angle);
		SetPos(newPos.x, newPos.y, 0);

		if (angle >= 265.9f &&angle <= 274.1f)
		{
			m_RotateSpeed = 0.0f;
			//SetAngle(270.0f);
		}
	}

	boxCollider->Update();
}