#include "Bullet.h"
#include <cstdio>
#include "iostream"
void Bullet::Init()
{
	// コンソールを割り当てる
	/*AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);*/
}

void Bullet::Update(void)
{
	DirectX::XMFLOAT3 direction;
	float pitch = shootAngle.y;
	float yaw = shootAngle.x+90;
	// 弾の方向を計算 (yaw: 水平角度, pitch: 垂直角度)
	direction.x = cos(DirectX::XMConvertToRadians(pitch)) * cos(DirectX::XMConvertToRadians(yaw));
	direction.y = sin(DirectX::XMConvertToRadians(pitch));
	direction.z = cos(DirectX::XMConvertToRadians(pitch)) * sin(DirectX::XMConvertToRadians(yaw));

	DirectX::XMFLOAT3 pos=GetPos();
	// 現在の位置に移動量を加算
	pos.x -= direction.x * speed;
	pos.y -= direction.y * speed;
	pos.z += direction.z * speed;

	// 弾の新しい位置を設定
	SetPos(pos.x, pos.y, pos.z);
	if (-1.0f < pos.z && pos.z < 1.0f)
	{

	}
}

void Bullet::Draw(void)
{
	//Render();      //プレイヤーを描画
}