#include "DontDragStage.h"

DontDragStage::DontDragStage()
{
	AddComponent("BoxCollider");
	SetObjectTexName("asset/Ground.jpg");
	SetObjTypeName("Ground");
	SetName("DontDragStage");
}

DontDragStage::~DontDragStage()
{

}


void DontDragStage::Init()//初期化
{
	Initialize(GetObjectTexName());   //背景を初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void DontDragStage::Update()//更新
{
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void DontDragStage::Draw()//描画
{

}