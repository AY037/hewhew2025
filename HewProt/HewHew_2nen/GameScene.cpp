#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
#include "GameManager.h"
#include "Engine.h"
#include "UI.h"
#include "Explosion.h"

//オブジェクトID
//#define PLAYER_ID (3)
//#define SWORD_ID (4)

using namespace DirectX;

void GameScene::Init()// シーンの初期化。
{
	//シーンのロード
	std::string txtName = sceneName + std::string(".txt");
	saveload.LoadScene(txtName, gameObjects, gameObjectList);


	for (auto& obj : gameObjectList)
	{
		if ((*obj)) (*obj)->Init();
	}

	for (auto& obj : gameObjectList)
	{
		if ((*obj)->GetName() == "Player") PLAYER_ID = (*obj)->GetObjID();
		if ((*obj)->GetName() == "DragSword") DRAGSWORD_ID = (*obj)->GetObjID();
	}

	camera.Init();

	//int size = GameManager::GetInstance().GetPlayerHP();
	//for (int i = 1; i <= size; ++i)
	//{
	//	std::shared_ptr<UI> heart=std::make_shared<UI>();
	//	heart->SetSize(SCREEN_WIDTH / 20, SCREEN_WIDTH / 20, 0);
	//	heart->SetPos(-SCREEN_WIDTH / 2 + SCREEN_WIDTH / 20 * i, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 20 * 3), 0);
	//	heart->Initialize("asset/Heart.png");
	//	uiObjectList.push_back(heart);
	//}
	hpBar.Init();
	backGrounds.Init();

	//Collieder二分木初期化
	DynamicAABBTree::GetInstance().Init(gameObjects);
	DynamicAABBTree::GetInstance().SetLeaf();

#ifdef GUI_MODE
	Engine::GetInstance().GetGuiController().Init(this, sceneName);
#endif

	SetEventManager();
}


void GameScene::Update()// シーン内のオブジェクト更新。
{
	DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
	if (hit_stop != 0)
	{
		stop_cnt++;
	}
	if (stop_cnt == hit_stop)
	{
		camera.Update(playerPos, false);
		hit_stop = 0;
		stop_cnt = 0;
		//Colliderの二分岐アップデート
		DynamicAABBTree::GetInstance().Update();

		for (auto& obj : gameObjectList)
		{
			if ((*obj))
			{
				if ((*obj)->GetPos().x > playerPos.x - 100 && (*obj)->GetPos().x < playerPos.x + 300)
				{
					(*obj)->Update();
				}

				// 画面外にいったオブジェクトの消去
				//if ((*obj)->GetPos().x < playerPos.x - 100)
				//{
				//	if ((*obj)->GetObjectType() == true)
				//	{
				//		removeObjects.push_back(pair.first); // 削除予定リストに追加
				//	}
				//}
			}
		}


		if ((input.GetRightTriggerPress()) || (input.GetKeyPress(VK_SPACE)))
		{
			gameObjects[DRAGSWORD_ID]->SetPos(playerPos.x - 17, playerPos.y, 0);
		}
		else
		{
			gameObjects[DRAGSWORD_ID]->SetPos(playerPos.x, 300, 0);//引きずっていないときは画面外へ
		}

		//オブジェクトの追加と消去
		AddAndDelete();
	}
	else
	{
		camera.Update(playerPos, true);
	}

	hpBar.Update();
	backGrounds.Update();
}


void GameScene::Draw()// シーン内のオブジェクトを描画
{
	//ビュー変換行列
	vm = camera.SetViewMatrix();
	// プロジェクション変換行列を作成
	pm = camera.SetProjectionMatrix();

	backGrounds.Draw(pm);

	for (auto& obj : gameObjectList)
	{
		if ((*obj)) {
			(*obj)->DrawObject(vm, pm);

		}
	}
	//プレイヤーのアニメーション描画
	gameObjects[PLAYER_ID]->Draw(vm, pm);
	//UIの描画
	for (auto& obj : uiObjectList)
	{
		if (obj) {
			obj->DrawUiObject(pm);
		}
	}
	hpBar.Draw(pm);
}


void GameScene::Shutdown()// シーンの終了処理。
{
	//二分木のリセット
	DynamicAABBTree::GetInstance().reset();
	StageCollider::GetInstance()->reset();

	sound.Stop(SOUND_LABEL_BGM000); //BGMを停止
}


void GameScene::SetEventManager()
{
	EventManager::GetInstance().AddObjectIdEvent("Shoot", [this](const int objId) {
		std::shared_ptr<GameObject> obj = GameObjectManager::GetInstance().GetObj("Bullet");
		DirectX::XMFLOAT3 pos = gameObjects[objId]->GetPos();
		obj->SetPos(pos.x, pos.y, pos.z);
		this->AddObject(obj);
		});

	EventManager::GetInstance().AddObjectIdEvent("Delete", [this](const int objId) {AddRemoveObject(objId); });

	//Enemyを残骸に
	EventManager::GetInstance().AddObjectIdEvent("TransDebri", [this](const int objId) {
		gameObjects[objId]->SetSize(7.0f, 7.0f, 0.0f);
		gameObjects[objId]->SetName("Debri");
		gameObjects[objId]->SetObjTypeName("Debri");
		gameObjects[objId]->SetIsBoxColl(true);

		std::shared_ptr<GameObject> obj = std::make_shared<Explosion>();
		DirectX::XMFLOAT3 pos = gameObjects[objId]->GetPos();
		obj->SetPos(pos.x, pos.y, 0.0f);
		AddObject(obj);
		});

	EventManager::GetInstance().AddObjectIdEvent("Explosion", [this](const int objId) {
		std::shared_ptr<GameObject> obj = std::make_shared<Explosion>();
		DirectX::XMFLOAT3 pos = gameObjects[objId]->GetPos();
		obj->SetPos(pos.x, pos.y, 0.0f);
		AddObject(obj);
		});

	//剣当たり判定の消去
	EventManager::GetInstance().AddListener("deleteSword", [this]() {
		std::vector<int> tmp = this->FindObjID("Sword");
		if (tmp.size() != 0)
		{
			this->AddRemoveObject(tmp.front());
		}
		});


	EventManager::GetInstance().AddListener("normalAttack", [this]()
		{
			std::vector<int> tmp = FindObjID("Sword");
			if (tmp.size() == 0)
			{
				DirectX::XMFLOAT3 velocity = { 4.0f, 1.0f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 16);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x + (gameObjects[PLAYER_ID]->GetSize().x / 2), gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 50, 0);
				obj->SetAngle(20);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}
		});

	EventManager::GetInstance().AddListener("attack1", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 4.0f, 1.2f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 25);
				obj->SetVelocity(velocity);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x + (gameObjects[PLAYER_ID]->GetSize().x / 2), gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 40, 0);
				obj->SetAngle(20);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}

			{
				DirectX::XMFLOAT3 velocity = { 6.0f, 3.0f, 0.0f };
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					DirectX::XMFLOAT3 playerSize = gameObjects[PLAYER_ID]->GetSize();
					//引きずってる剣の近くのオブジェクトのみ集める
					if ((pos.x > playerPos.x - 80 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(pos.x + playerSize.x / 2 + 25, pos.y + 25, 0);
						//gameObjects[objID]->SetPos(pos.x + playerSize.x / 2, pos.y + 5, 0);
						//gameObjects[objID]->SetVelocity(velocity);
					}
				}
			}
			hit_stop = 15;//ヒットストップの継続時間
		});

	EventManager::GetInstance().AddListener("attack2", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 5.3f, 1.5f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 25);
				obj->SetVelocity(velocity);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x + (gameObjects[PLAYER_ID]->GetSize().x / 2), gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 60, 0);
				obj->SetAngle(20);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}

			{
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 velocity = { 9.0f, 4.5f, 0.0f };
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					DirectX::XMFLOAT3 playerSize = gameObjects[PLAYER_ID]->GetSize();
					//引きずってる剣の近くのオブジェクトのみ集める
					if ((pos.x > playerPos.x - 80 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(pos.x + playerSize.x / 2+25, pos.y + 25, 0);
						//gameObjects[objID]->SetPos(pos.x + playerSize.x / 2, pos.y + 5, 0);
						//gameObjects[objID]->SetVelocity(velocity);
					}
				}
			}
			hit_stop = 25;//ヒットストップの継続時間
		});
}
