#include "Sword.h"
#include "EventManager.h"
void Sword::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 3, 4); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
}

void Sword::Update(void)
{
	DirectX::XMFLOAT3 size = GetSize();
	falme_cnt++;

	DirectX::XMFLOAT3 _pos = GetPos();

	SetVelocity(swordSpeed);
	SetPos(_pos.x + swordSpeed.x, _pos.y + swordSpeed.y, 0);

	for (auto& component : components)
	{
		component.second->Update();
	}

	if (falme_cnt == alive_time)
	{
		EventManager::GetInstance().SendEvent("deleteSword");
	}
	SetSize(size.x, size.y * 1.02, 0);
}

void Sword::Draw(void)
{
	//Render();      //プレイヤーを描画
}