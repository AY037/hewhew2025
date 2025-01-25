#include "FallGround.h"
#include "EventManager.h"

FallGround::FallGround()
{
	rigidbody.Init(*this);
	SetObjectTexName("asset/block.png");
	SetObjTypeName("Ground");
	//isRigidbody = true;
	SetSize(10, 10, 0);
	SetAngle(0);
}

FallGround::~FallGround()
{

}

void FallGround::Init()
{
	Initialize(GetObjectTexName()); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	boxColl.Init(*this);
}

void FallGround::Update()
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);

	auto hitObjects = boxColl.HitObjectName(*this);

	for (auto& obj : hitObjects)
	{
		std::string objTypeName = obj->GetObjTypeName();
		if (objTypeName == "DragSword")
		{
 			m_ActiveRigidbody = true;
		}
		if (objTypeName == "Enemy")
		{
			int id = obj->GetObjID();
			EventManager::GetInstance().SendObjIdEvent("Explosion", id);
			EventManager::GetInstance().SendObjIdEvent("TransDebri", id);
		}
		if (objTypeName == "Ground"|| objTypeName == "Stage")
		{
			m_ActiveBoxcollider = true;
		}
	}

	if (m_ActiveRigidbody)
	{
		rigidbody.Update();
	}
	if (m_ActiveBoxcollider)
	{
		boxColl.Update();
	}
}