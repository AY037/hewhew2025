#include "HpBar.h"
#include "GameManager.h"

HpBar::HpBar()//コンストラクタ
{
}

HpBar::~HpBar()//デストラクタ
{
	Uninit();
}

void HpBar::Init()
{
	chargeBar.SetPos(-SCREEN_WIDTH / 2 + 280, SCREEN_HEIGHT / 2 - 117, 0.0f);//位置を指定
	chargeBar.SetSize(SCREEN_WIDTH / 7 - 20, SCREEN_WIDTH / 7 - 20, 0.0f);//大きさを指定
	chargeBar.Initialize("asset/charge.png");
	chargeBar.SetAngle(-36);


	std::unique_ptr<UI> hpBar = std::make_unique<UI>();

	hpBar->SetPos(-SCREEN_WIDTH/2 + 130, SCREEN_HEIGHT/2 - 20, 0.0f);//位置を指定
	hpBar->SetSize(SCREEN_WIDTH / 8-50, (SCREEN_WIDTH / 4)/8-10, 0.0f);//大きさを指定
	hpBar->SetObjectTexName("asset/hpBar.png");
	hpBar->Init();
	textures["HpBar"] = std::move(hpBar);

	std::unique_ptr<UI> hpFlame = std::make_unique<UI>();
	hpFlame->SetPos(-SCREEN_WIDTH / 2 + 240, SCREEN_HEIGHT / 2 - 80, 0.0f);//位置を指定
	hpFlame->SetSize(SCREEN_WIDTH/4, SCREEN_WIDTH / 8, 0.0f);//大きさを指定
	hpFlame->SetObjectTexName("asset/HpFlame.png");
	hpFlame->Init();
	textures["HpFlame"] = std::move(hpFlame);
	//std::unique_ptr<GameObject> hp = std::make_unique<GameObject>();

	//hp->SetTexture("assets/texture/hp.png");//画像の指定
	//hp->SetPos(-960 + 65, 540 - 60, 0.0f);//位置を指定
	//hp->SetSize(60, 30.0f, 0.0f);//大きさを指定
	//hp->Init();
	//textures["Hp"] = std::move(hp);
}

void HpBar::Update()
{
	auto& gameManager = GameManager::GetInstance();

	std::unique_ptr<UI>& hpBar = textures["HpBar"];
	float playerHp = gameManager.GetPlayerHP();
	float hpBarSize = 380.0f * (playerHp / 300.0f);

	hpBar->SetSize(hpBarSize, (SCREEN_WIDTH / 4) / 8, 0.0f);
	hpBar->SetPos(-SCREEN_WIDTH / 2 + hpBarSize / 2 + 90, SCREEN_HEIGHT / 2 - 50, 0.0f);

}

void HpBar::Draw(DirectX::XMMATRIX& _pm)
{
	//チャージバー描画
	//===============================
	int chargeEnergy = GameManager::GetInstance().dragCharging;
	int sx = static_cast<int>(14.0f * (chargeEnergy / 180.0f));

	for (int i = 0; i < sx; ++i)
	{
		chargeBar.SetPos(-SCREEN_WIDTH / 2 + 180 + i * 15, SCREEN_HEIGHT / 2 - 50 - i * 10, 0.0f);//位置を指定
		chargeBar.SetSize(20, 250.0f, 0);//大きさを指定
		chargeBar.SetUV(i, 0, 14, 1);
		chargeBar.DrawUiObject(_pm);
	}
	//===============================

	for (auto& obj : textures)
	{
		obj.second->DrawUiObject(_pm);
	}

}

void HpBar::Uninit()
{
	for (auto& obj : textures)
	{
		obj.second->Uninit();
	}
	textures.clear();
}