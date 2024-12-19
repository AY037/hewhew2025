#include "Stage.h"
void Stage::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager);   //�w�i��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void Stage::Update(void)
{
	for (auto& component : components)
	{
		component.second->Update();
	}
}

void Stage::Draw(void)
{

}