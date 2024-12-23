#include "DragSword.h"
void DragSword::Init(TextureManager& _textureManager)
{
	//Initialize(GetObjectTexName(), _textureManager);   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void DragSword::Update(void)
{
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void DragSword::Draw(void)
{

}