#include "Stage.h"
void Stage::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager);   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Stage::Update(void)
{
	for (auto& component : components)
	{
		component->Update();
	}
}

void Stage::Draw(void)
{

}