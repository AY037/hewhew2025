#include "Bullet.h"
void Bullet::Init()
{

}

void Bullet::Update(void)
{
	DirectX::XMFLOAT3 pos = GetPos();

	pos.x++;
	SetPos(pos.x, pos.y, pos.z);
}

void Bullet::Draw(void)
{
	//Render();      //ƒvƒŒƒCƒ„[‚ğ•`‰æ
}