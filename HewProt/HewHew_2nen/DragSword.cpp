#include "DragSword.h"
#include "GameManager.h"

void DragSword::Init()
{
	//Initialize(GetObjectTexName());   //”wŒi‚ð‰Šú‰»
	//SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	boxCollider.Init(*this);
}

void DragSword::Update(void)
{
	auto& gameManager = GameManager::GetInstance();
	auto hitObjects = boxCollider.HitObjectName(*this);
	if (hitObjects.size()>1)
	{
		gameManager.dragSwordHit = true;
	}
	else
	{
		gameManager.dragSwordHit = false;
	}
}

void DragSword::Draw(void)
{

}