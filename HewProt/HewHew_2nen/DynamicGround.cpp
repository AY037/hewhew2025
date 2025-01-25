#include "DynamicGround.h"
#include "EventManager.h"

DynamicGround::DynamicGround()
{
	SetObjectTexName("asset/block.png");
	SetObjTypeName("Ground");
	isRigidbody = true;
	SetSize(10, 10, 0);
	SetAngle(0);
}

DynamicGround::~DynamicGround()
{

}

void DynamicGround::Init()
{
	Initialize(GetObjectTexName()); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	boxColl.Init(*this);
	rigidbody.Init(*this);
}

void DynamicGround::Update()
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);

	rigidbody.Update();
	boxColl.Update();
}