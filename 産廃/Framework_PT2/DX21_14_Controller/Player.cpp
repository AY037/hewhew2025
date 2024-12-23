#include "Player.h"
#include "EventManager.h"
void Player::Init(TextureManager& _textureManager)
{
	Initialize(L"asset/char01.png",  _textureManager, 3, 4); //プレイヤーを初期化
	SetPos(0.0f, 0.0f, -101.0f);      //位置を設定
	SetSize(20.0f, 30.0f, 0.0f);	 //大きさを設定
	SetAngle(0.0f);                  //角度を設定
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定

	EventManager::GetInstance().AddListener("PlayerGetPos", [this]() {
		this->GetPos(); });
}

void Player::Update(void)
{
	//プレイヤーのアニメーション
	numU++;
	if (numU >= 3) {
		numU = 0;
	}
	DirectX::XMFLOAT3 pos = GetPos();

	//if (input.GetKeyPress(VK_W)) { pos.z += 1.0f; }
	//if (input.GetKeyPress(VK_A)) { pos.x -= 1.0f; }
	//if (input.GetKeyPress(VK_S)) { pos.z -= 1.0f; }
	//if (input.GetKeyPress(VK_D)) { pos.x += 1.0f; }
	//if (input.GetKeyPress(VK_SPACE)) {
	//	EventManager::GetInstance().SendEvent("shoot"); 
	//}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		EventManager::GetInstance().SendEvent("shoot");
	}
	SetPos(pos.x, pos.y, pos.z);
}

void Player::Draw(void)
{
	//Render();      //プレイヤーを描画
}