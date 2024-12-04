#include "Background.h"
void Background::Init(TextureManager& _textureManager)
{
	Initialize("asset/block.png", _textureManager);   //”wŒi‚ð‰Šú‰»
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//Šp“x‚ðÝ’è
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Background::Update(void)
{
	for (auto& component : components)
	{
		component->Update();
	}
	//DirectX::XMFLOAT3 _pos =GetPos();
	//DirectX::XMFLOAT3 _velocity = GetVelocity();

	//SetPos(_pos.x + _velocity.x, _pos.y+ _velocity.y, _pos.z + _velocity.z);
}

void Background::Draw(void)
{
	
}