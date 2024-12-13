#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
#include "Engine.h"

//オブジェクトID
#define PLAYER_ID (3)
#define SWORD_ID (4)

using namespace DirectX;

void GameScene::Init()// シーンの初期化。
{
	camera->Init();

	//シーンのロード
	saveload.LoadScene("GameScene2.txt", gameObjects,textureManager);

	//オブジェクトの初期化
	for (auto& obj : gameObjects)
	{
		if (obj.second) obj.second->Init(textureManager);
	}

	//Collieder二分木初期化
	DynamicAABBTree::GetInstance().Init(gameObjects);
	DynamicAABBTree::GetInstance().SetLeaf();

#ifdef GUI_MODE
	Engine::GetInstance().GetGuiController().Init(this);
#endif
	EventManager::GetInstance().AddListener("normalAttack", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 3.0f, 3.0f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity);
				obj->SetName("Sword");
				int objID = gameObjects.size();
				obj->SetObjID(objID);
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 10, gameObjects[PLAYER_ID]->GetPos().y - 25, 0);
				obj->SetSize(20, 30, 0);
				obj->SetAngle(30);
				obj->SetObjectTexName("asset/block.png");
				obj->Init(textureManager);
				this->AddObject(obj);
			}
			slash_height = 10.0f;//斬撃の高さを設定
		});

	EventManager::GetInstance().AddListener("attack1", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 4.5f, 4.5f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity);
				obj->SetName("Sword");
				int objID = gameObjects.size();
				obj->SetObjID(objID);
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x+10, gameObjects[PLAYER_ID]->GetPos().y - 25, 0);
				obj->SetSize(20, 30, 0);
				obj->SetAngle(30);
				obj->SetObjectTexName("asset/block.png");
				obj->Init(textureManager);
				this->AddObject(obj);
			}

			{
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					//引きずってる剣の近くのオブジェクトのみ集める
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20)&& (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 70, gameObjects[PLAYER_ID]->GetPos().y + 20, 0);
					}
				}
			}
			hit_stop = 10;//ヒットストップの継続時間
			slash_height = 20.0f;//斬撃の高さを設定
		});

	EventManager::GetInstance().AddListener("attack2", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 6.0f, 6.0f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity);
				obj->SetName("Sword");
				int objID = gameObjects.size();
				obj->SetObjID(objID);
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 20, gameObjects[PLAYER_ID]->GetPos().y-30, 0);
				obj->SetSize(20, 50, 0);
				obj->SetAngle(30);
				obj->SetObjectTexName("asset/block.png");
				obj->Init(textureManager);
				this->AddObject(obj);
			}

			{
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					//引きずってる剣の近くのオブジェクトのみ集める
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 70, gameObjects[PLAYER_ID]->GetPos().y + 30, 0);
					}
				}
			}
			hit_stop = 20;//ヒットストップの継続時間
			slash_height = 30.0f;//斬撃の高さを設定
		});
}
void GameScene::Update()// シーン内のオブジェクト更新。
{
	DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
	if (hit_stop != 0)
	{
		stop_cnt++;
	}
	if(stop_cnt==hit_stop)
	{
		hit_stop = 0;
		stop_cnt = 0;
		//Colliderの二分岐アップデート
		DynamicAABBTree::GetInstance().Update();
		//DynamicAABBTree::GetInstance().updateDebugFlame();


		std::vector<int> toDelete;

		for (auto& pair : gameObjects)
		{
			if (pair.second)
			{
				pair.second->Update();

				// 画面外にいったオブジェクトの消去
				if (pair.second->GetPos().x < playerPos.x - 100)
				{
					if (pair.second->GetObjectType() == true)
					{
						toDelete.push_back(pair.first); // 削除予定リストに追加
					}
				}
			}
		}

		// ループ終了後に削除を実行
		for (const auto& key : toDelete)
		{
			//DeleteObject(key);
		}


		if (input.GetKeyPress(VK_SPACE))
		{
			gameObjects[SWORD_ID]->SetPos(playerPos.x - 17, playerPos.y, 0);
		}
		else
		{
			gameObjects[SWORD_ID]->SetPos(playerPos.x, 300, 0);//引きずっていないときは画面外へ
		}

		std::vector<int> ids = FindObjID("Sword");
		if (ids.size() != 0)
		{

			for (int objID : ids)
			{
				//斬撃がある程度の高さになったら消去
				if (gameObjects[objID]->GetPos().y > playerPos.y + slash_height)
				{
					DeleteObject(objID);
				}
			}
		}
		camera->Update(playerPos);
	}
	else
	{
		camera->Update(playerPos,true);
	}
}


void GameScene::Draw()// シーン内のオブジェクトを描画
{
	//ビュー変換行列
	vm = camera->SetViewMatrix();
	// プロジェクション変換行列を作成
	pm = camera->SetProjectionMatrix();

	for (auto& pair : gameObjects)
	{
		if (pair.second) {
			if (pair.second->GetName() != "DragSword")
			{
				pair.second->DrawObject(vm, pm);
			}
		}
	}
	//プレイヤーのアニメーション描画
	gameObjects[PLAYER_ID]->Draw(vm, pm);
	//debug.DrawNode(vm, pm);

}


void GameScene::Shutdown()// シーンの終了処理。
{
}