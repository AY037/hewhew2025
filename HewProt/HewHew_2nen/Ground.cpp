#include "Ground.h"
void Ground::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager);   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Ground::Update(void)
{
	for (auto& component : components)
	{
		component->Update();
	}
}

void Ground::Draw(void)
{

}