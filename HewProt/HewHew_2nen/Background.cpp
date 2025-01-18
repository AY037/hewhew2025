#include "Background.h"
#include "GameManager.h"

Background::Background()
{
}

Background::~Background()
{
	backGrounds.clear();
}

void Background::Init()
{
	//”wŒi‚P
	for (int i = 11; i >= 0; --i)
	{
		std::unique_ptr<UI> backGround = std::make_unique<UI>();
		backGround->SetPos(0.0f, 0.0f, 0.0f);//ˆÊ’u‚ðŽw’è
		backGround->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);//‘å‚«‚³‚ðŽw’è

		if (i % 2 == 1)
		{
			backGround->SetPos(SCREEN_WIDTH, 0.0f, 0.0f);//ˆÊ’u‚ðŽw’è
		}

		//”wŒiƒ‹[ƒv‚ðì‚é‚½‚ß‚É“ñ–‡—pˆÓ
		switch (i)
		{
		case 0:
			backGround->SetObjectTexName("asset/Background/6.png");
			break;
		case 1:
			backGround->SetObjectTexName("asset/Background/6.png");
			break;
		case 2:
			backGround->SetObjectTexName("asset/Background/5.png");
			break;
		case 3:
			backGround->SetObjectTexName("asset/Background/5.png");
			break;
		case 4:
			backGround->SetObjectTexName("asset/Background/4.png");
			break;
		case 5:
			backGround->SetObjectTexName("asset/Background/4.png");
			break;
		case 6:
			backGround->SetObjectTexName("asset/Background/3.png");
			break;
		case 7:
			backGround->SetObjectTexName("asset/Background/3.png");
			break;
		case 8:
			backGround->SetObjectTexName("asset/Background/2.png");
			break;
		case 9:
			backGround->SetObjectTexName("asset/Background/2.png");
			break;
		case 10:
			backGround->SetObjectTexName("asset/Background/1.png");
			break;
		case 11:
			backGround->SetObjectTexName("asset/Background/1.png");
			break;
		default:
			break;
		}

		backGround->Init();

		backGrounds.push_back(std::move(backGround));
	}
}

void Background::Update(void)
{
	for (int i = 0; i < 12; ++i)
	{
		DirectX::XMFLOAT3 pos;

		int index = i;

		if (i % 2 == 1)
		{
			index -= 1;
		}

		switch (index)
		{
		case 0:
			pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x - 0.5f, pos.y, pos.z);
			break;
		case 2:
			 pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x - 1.0f, pos.y, pos.z);
			break;
		case 4:
			 pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x - 1.5f, pos.y, pos.z);
			break;
		case 6:
			 pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x - 2.0f, pos.y, pos.z);
			break;
		case 8:
			 pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x - 2.5f, pos.y, pos.z);
			break;
		case 10:
			 pos = backGrounds[i]->GetPos();
			backGrounds[i]->SetPos(pos.x-3.0f, pos.y, pos.z);
			break;
		default:
			break;
		}

		//”wŒi‚ªŒ©‚¦‚È‚­‚È‚Á‚½‚çˆÚ“®
		pos = backGrounds[i]->GetPos();
		DirectX::XMFLOAT3 size = backGrounds[i]->GetSize();
		if (pos.x + size.x / 2 < -SCREEN_WIDTH / 2)
		{
			backGrounds[i]->SetPos(SCREEN_WIDTH, pos.y, pos.z);
		}
	}
}

void Background::Draw(DirectX::XMMATRIX& _pm)
{
	for (auto& obj : backGrounds)
	{
		obj->DrawUiObject(_pm);
	}
}