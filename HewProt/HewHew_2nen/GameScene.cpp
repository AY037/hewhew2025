#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
#include "Engine.h"

//�I�u�W�F�N�gID
//#define PLAYER_ID (3)
//#define SWORD_ID (4)

using namespace DirectX;

void GameScene::Init()// �V�[���̏������B
{
	camera->Init();

	//�V�[���̃��[�h
	saveload.LoadScene("GameScene2.txt", gameObjects, gameObjectList, textureManager);

	for (auto& obj : gameObjectList)
	{
		if ((*obj)) (*obj)->Init(textureManager);
	}

	for (auto& obj : gameObjectList)
	{
		if ((*obj)->GetName()=="Player") PLAYER_ID= (*obj)->GetObjID();
		if ((*obj)->GetName() == "DragSword") DRAGSWORD_ID = (*obj)->GetObjID();
	}
	//�I�u�W�F�N�g�̏�����
	//for (auto& obj : gameObjects)
	//{
	//	if (obj.second) obj.second->Init(textureManager);
	//}

	//Collieder�񕪖؏�����
	DynamicAABBTree::GetInstance().Init(gameObjects);
	DynamicAABBTree::GetInstance().SetLeaf();

#ifdef GUI_MODE
	Engine::GetInstance().GetGuiController().Init(this);
#endif

	AddEventManager();
	Sound::GetInstance().Play(SOUND_LABEL_BGM001);
}


void GameScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
{
	DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
	if (hit_stop != 0)
	{
		stop_cnt++;
	}
	if (stop_cnt == hit_stop)
	{
		camera->Update(playerPos, false);
		hit_stop = 0;
		stop_cnt = 0;
		//Collider�̓񕪊�A�b�v�f�[�g
		DynamicAABBTree::GetInstance().Update();
		//DynamicAABBTree::GetInstance().updateDebugFlame();

		for (auto& obj : gameObjectList)
		{
			if ((*obj))
			{
				(*obj)->Update();

				// ��ʊO�ɂ������I�u�W�F�N�g�̏���
				//if ((*obj)->GetPos().x < playerPos.x - 100)
				//{
				//	if ((*obj)->GetObjectType() == true)
				//	{
				//		removeObjects.push_back(pair.first); // �폜�\�胊�X�g�ɒǉ�
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
			gameObjects[DRAGSWORD_ID]->SetPos(playerPos.x, 300, 0);//���������Ă��Ȃ��Ƃ��͉�ʊO��
		}

		for (const auto& id : addObjects)
		{
			gameObjectList.push_back(&gameObjects[id]);
		}
		addObjects.clear();
		// ���[�v�I����ɍ폜�����s
		for (const auto& key : removeObjects)
		{
			DeleteObject(key);
		}
		removeObjects.clear();
	}
	else
	{
		camera->Update(playerPos, true);
	}
}


void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
	//�r���[�ϊ��s��
	vm = camera->SetViewMatrix();
	// �v���W�F�N�V�����ϊ��s����쐬
	pm = camera->SetProjectionMatrix();

	for (auto& obj : gameObjectList)
	{
		if ((*obj)) {
			(*obj)->DrawObject(vm, pm);

		}
	}
	//�v���C���[�̃A�j���[�V�����`��
	gameObjects[PLAYER_ID]->Draw(vm, pm);
	//debug.DrawNode(vm, pm);

}


void GameScene::Shutdown()// �V�[���̏I�������B
{
}

void GameScene::AddEventManager()
{
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
			if (tmp.size() != 0)
			{
			}
			else
			{
				DirectX::XMFLOAT3 velocity = { 3.0f, 1.1f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 30);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x, gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 50, 0);
				obj->SetAngle(30);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}
		});

	EventManager::GetInstance().AddListener("attack1", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 3.5f, 1.4f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 30);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x, gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 40, 0);
				obj->SetAngle(30);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}

			{
				DirectX::XMFLOAT3 velocity = { 9.0f, 4.5f, 0.0f };
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					//���������Ă錕�̋߂��̃I�u�W�F�N�g�̂ݏW�߂�
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						//gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 30, gameObjects[PLAYER_ID]->GetPos().y + 10, 0);
						gameObjects[objID]->SetVelocity(velocity);
					}
				}
			}
			hit_stop = 10;//�q�b�g�X�g�b�v�̌p������
		});

	EventManager::GetInstance().AddListener("attack2", [this]()
		{
			{
				DirectX::XMFLOAT3 velocity = { 6.0f, 2.0f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 30);
				obj->SetName("Sword");
				obj->SetPos(gameObjects[PLAYER_ID]->GetPos().x, gameObjects[PLAYER_ID]->GetPos().y, 0);
				obj->SetSize(20, 50, 0);
				obj->SetAngle(30);
				//obj->SetObjectTexName("asset/block.png");
				this->AddObject(obj);
			}

			{
				std::vector<int> ids = FindObjID("Debri");
				for (int objID : ids)
				{
					DirectX::XMFLOAT3 velocity = { 12.0f, 6.0f, 0.0f };
					DirectX::XMFLOAT3 pos = gameObjects[objID]->GetPos();
					DirectX::XMFLOAT3 playerPos = gameObjects[PLAYER_ID]->GetPos();
					//���������Ă錕�̋߂��̃I�u�W�F�N�g�̂ݏW�߂�
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						//gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 30, gameObjects[PLAYER_ID]->GetPos().y + 10, 0);
						gameObjects[objID]->SetVelocity(velocity);
					}
				}
			}
			hit_stop = 20;//�q�b�g�X�g�b�v�̌p������
		});

}