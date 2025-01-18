#include "Option.h"
#include "GameManager.h"
#include "EventManager.h"
#include "Application.h"
#include "Input.h"
#include "AudioManager.h"

using namespace DirectX::SimpleMath;

Option::Option(Camera* _camera)//コンストラクタ
{
	camera = _camera;

	RECT rect = Application::GetClientWindowRect();
	screenLeft = -(rect.left + (rect.right - rect.left) / 2);
	screenTop = rect.top + (rect.bottom - rect.top) / 2;
	
	topPingPosition = Vector3(161, -285.8 + screenTop - 3, 0.0f);//位置を指定
	bottomPingPosition = Vector3(161, -540.8 + screenTop + 3, 0.0f);//位置を指定
}

Option::~Option()//デストラクタ
{
	Uninit();
}

void Option::Init()
{
	auto& eventManager = EventManager::GetInstance();

	RECT rect = Application::GetClientWindowRect();
	screenLeft = -(rect.left + (rect.right - rect.left) / 2);
	screenTop = rect.top + (rect.bottom - rect.top) / 2;
	//背景オブジェクト
	//======================================================================================
	std::shared_ptr<Texture2D> option = std::make_unique<Texture2D>(camera);

	option->SetTexture("assets/texture/option.png");//画像の指定
	option->SetPosition(0, 0, 0.0f);//位置を指定
	option->SetScale(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);//大きさを指定
	option->Init();
	texturesParent["option"] = option;
	textures.push_back(texturesParent["option"]);
	//======================================================================================

	//調整する＜＞オブジェクト
	//======================================================================================
	std::shared_ptr<Texture2D> select[4];
	for (int i = 0; i < 4; ++i)
	{
		select[i] = std::make_unique<Texture2D>(camera);
		select[i]->SetTexture("assets/texture/select.png");//画像の指定
		select[i]->SetScale(120, 120, 0.0f);//大きさを指定
		select[i]->Init();
	}

	//左上
	select[0]->SetPosition(607.8 + screenLeft, -285.8 + screenTop, 0.0f);//位置を指定
	select[0]->SetUV(0, 0, 2, 2);
	texturesParent["selectLeftTop"] = std::move(select[0]);
	textures.push_back(texturesParent["selectLeftTop"]);

	//左下
	select[1]->SetPosition(607.8 + screenLeft, -540.8 + screenTop, 0.0f);//位置を指定
	select[1]->SetUV(0, 0, 2, 2);
	texturesParent["selectLeftBottom"] = select[1];
	textures.push_back(texturesParent["selectLeftBottom"]);

	//右上
	select[2]->SetPosition(1654.2f + screenLeft, -285.8 + screenTop, 0.0f);//位置を指定
	select[2]->SetUV(1, 0, 2, 2);
	texturesParent["selectRightTop"] = select[2];
	textures.push_back(texturesParent["selectRightTop"]);

	//右下
	select[3]->SetPosition(1654.2f + screenLeft, -540.8 + screenTop, 0.0f);//位置を指定
	select[3]->SetUV(1, 0, 2, 2);
	texturesParent["selectRightBottom"] = select[3];
	textures.push_back(texturesParent["selectRightBottom"]);
	//======================================================================================

	//調整ピンオブジェクト
	//======================================================================================
	std::shared_ptr<Texture2D> ping[2];
	for (int i = 0; i < 2; ++i)
	{
		ping[i] = std::make_unique<Texture2D>(camera);
		ping[i]->SetTexture("assets/texture/ping.png");//画像の指定
		ping[i]->SetScale(20, 100, 0.0f);//大きさを指定
		ping[i]->Init();
	}

	ping[0]->SetPosition(topPingPosition.x, topPingPosition.y, 0.0f);//位置を指定
	texturesParent["pingTop"] = std::move(ping[0]);
	textures.push_back(texturesParent["pingTop"]);

	ping[1]->SetPosition(bottomPingPosition.x, bottomPingPosition.y, 0.0f);//位置を指定
	texturesParent["pingBottom"] = ping[1];
	textures.push_back(texturesParent["pingBottom"]);
	//======================================================================================


	//ゲームに戻る、タイトルに戻るオブジェクト
	//======================================================================================
	std::shared_ptr<Texture2D> game_or_title[2];
	for (int i = 0; i < 2; ++i)
	{
		game_or_title[i] = std::make_unique<Texture2D>(camera);
		game_or_title[i]->SetTexture("assets/texture/game_or_title.png");//画像の指定
		game_or_title[i]->SetScale(420, 120, 0.0f);//大きさを指定
		game_or_title[i]->Init();
	}

	game_or_title[0]->SetPosition(680.0f + screenLeft, -890 + screenTop, 0.0f);//位置を指定
	game_or_title[0]->SetUV(0, 0, 2, 2);
	texturesParent["returnTitle"] = std::move(game_or_title[0]);
	textures.push_back(texturesParent["returnTitle"]);

	game_or_title[1]->SetPosition(1340.0f + screenLeft, -890 + screenTop, 0.0f);//位置を指定
	game_or_title[1]->SetUV(1, 0, 2, 2);
	texturesParent["returnGame"] = game_or_title[1];
	textures.push_back(texturesParent["returnGame"]);
	//=======================================================================================

	//EventManager
	eventManager.AddListener("DrawOptionTrue", [this]() {
		drawActive = true;
		});

	eventManager.AddListener("DrawOptionFalse", [this]() {
		drawActive = false;
		});
}

void Option::Update()
{
	if (drawActive)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);

		Vector3 mouseWorldPos = Vector3(mousePos.x + screenLeft, -mousePos.y + screenTop, 0.0f);

		for (auto& pair : texturesParent)
		{
			auto& obj = pair.second;

			Vector3 pos = obj->GetPosition();
			Vector3 scale = obj->GetScale();

			Vector3 direction = mouseWorldPos - pos;

			//マウスカーソルがオブジェクトの中にあるか
			if (std::fabs(direction.x) < std::fabs(scale.x / 2) && std::fabs(direction.y) < std::fabs(scale.y / 2))
			{
				//＜＞オブジェクトのUV調整
				if (pair.first != "option" && pair.first != "pingTop" && pair.first != "pingBottom")
				{
					Vector4 uv = obj->GetUV();
					obj->SetUV(uv.x, 1, uv.z, uv.w);
				}
				if (Input::GetKeyTrigger(VK_LBUTTON))
				{
					AudioManager::GetInstance().StopAudio(SE_SELECT);
					AudioManager::GetInstance().PlayAudio(SE_SELECT, false, 3.0f);
					//ピンの移動
					//=================================================================================
					if (pair.first == "selectLeftTop")
					{
						Vector3 pingPos = texturesParent["pingTop"]->GetPosition();
						if (pingPos.x > -160.0f)
						{
							texturesParent["pingTop"]->SetPosition(pingPos.x - 66, pingPos.y, 0.0f);
							auto& audioManager = AudioManager::GetInstance();
							audioManager.SetMasterVolume(audioManager.GetMasterVolume() - 0.1f);
						}
					}
					if (pair.first == "selectRightTop")
					{
						Vector3 pingPos = texturesParent["pingTop"]->GetPosition();
						if (pingPos.x < 474.0f)
						{
							texturesParent["pingTop"]->SetPosition(pingPos.x + 66, pingPos.y, 0.0f);
							auto& audioManager = AudioManager::GetInstance();
							audioManager.SetMasterVolume(audioManager.GetMasterVolume() + 0.1f);
						}
					}

					if (pair.first == "selectLeftBottom")
					{
						Vector3 pingPos = texturesParent["pingBottom"]->GetPosition();
						if (pingPos.x > -160.0f)
						{
							texturesParent["pingBottom"]->SetPosition(pingPos.x - 66, pingPos.y, 0.0f);
							auto& gameManager = GameManager::GetInstance();
							gameManager.SetSensi(gameManager.GetSensi() - 0.1f);
						}
					}
					if (pair.first == "selectRightBottom")
					{
						Vector3 pingPos = texturesParent["pingBottom"]->GetPosition();
						if (pingPos.x < 474.0f)
						{
							texturesParent["pingBottom"]->SetPosition(pingPos.x + 66, pingPos.y, 0.0f);
							auto& gameManager = GameManager::GetInstance();
							gameManager.SetSensi(gameManager.GetSensi() + 0.1f);
						}
					}

					topPingPosition = texturesParent["pingTop"]->GetPosition();
					bottomPingPosition = texturesParent["pingBottom"]->GetPosition();
					//=================================================================================

					if (pair.first == "returnTitle")
					{
						EventManager::GetInstance().SendChangeScene("TitleScene");
						drawActive = false;
					}
					if (pair.first == "returnGame")
					{
						drawActive = false;
					}
				}
			}
			else
			{
				Vector4 uv = obj->GetUV();
				obj->SetUV(uv.x, 0, uv.z, uv.w);
			}

		}
	}

}

void Option::Draw()
{
	if (drawActive)
	{
		for (auto& obj : textures)
		{
			obj->Draw();
		}
	}
}

void Option::Uninit()
{
	for (auto& obj : textures)
	{
		obj->Uninit();
	}
	textures.clear();
	texturesParent.clear();
}


void Option::SetDrawActive(const bool _drawActive)
{
	drawActive = _drawActive;
}

bool Option::GetDrawActive()const
{
	return drawActive;
}