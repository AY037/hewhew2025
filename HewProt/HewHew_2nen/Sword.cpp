#include "Sword.h"
#include "EventManager.h"
void Sword::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 3, 4); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Sword::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();

	SetVelocity(swordSpeed);
	SetPos(_pos.x + swordSpeed.x, _pos.y + swordSpeed.y, 0);

	for (auto& component : components)
	{
		component->Update();
	}
}

void Sword::Draw(void)
{
	//Render();      //プレイヤーを描画
}