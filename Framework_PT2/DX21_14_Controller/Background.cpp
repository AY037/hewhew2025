#include "Background.h"
void Background::Init(TextureManager& _textureManager)
{
	Initialize(L"asset/back_img_01.png", _textureManager);   //”wŒi‚ğ‰Šú‰»
	SetPos(0.0f, 0.0f, 0.0f);      //ˆÊ’u‚ğİ’è
	SetSize(100.0f, 100.0f, 0.0f);  //‘å‚«‚³‚ğİ’è
	SetAngle(0.0f);                //Šp“x‚ğİ’è
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//Šp“x‚ğİ’è
}

void Background::Update(void)
{
	
}

void Background::Draw(void)
{
	
}