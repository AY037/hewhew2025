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


void DontDragStage::Init()//‰Šú‰»
{
	Initialize(GetObjectTexName());   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void DontDragStage::Update()//XV
{
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void DontDragStage::Draw()//•`‰æ
{

}