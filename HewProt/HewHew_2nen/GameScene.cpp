#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Sword.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
#include "Engine.h"

//�I�u�W�F�N�gID
#define PLAYER_ID (3)
#define SWORD_ID (4)

using namespace DirectX;

void GameScene::Init()// �V�[���̏������B
{
	camera->Init();

	//�V�[���̃��[�h
	saveload.LoadScene("GameScene2.txt", gameObjects,textureManager);

	//�I�u�W�F�N�g�̏�����
	for (auto& obj : gameObjects)
	{
		if (obj.second) obj.second->Init(textureManager);
	}

	//Collieder�񕪖؏�����
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
			slash_height = 10.0f;//�a���̍�����ݒ�
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
					//���������Ă錕�̋߂��̃I�u�W�F�N�g�̂ݏW�߂�
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20)&& (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 70, gameObjects[PLAYER_ID]->GetPos().y + 20, 0);
					}
				}
			}
			hit_stop = 10;//�q�b�g�X�g�b�v�̌p������
			slash_height = 20.0f;//�a���̍�����ݒ�
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
					//���������Ă錕�̋߂��̃I�u�W�F�N�g�̂ݏW�߂�
					if ((pos.x > playerPos.x - 50 && pos.x < playerPos.x + 20) && (pos.y > playerPos.y - 20 && pos.y < playerPos.y + 20))
					{
						gameObjects[objID]->SetPos(gameObjects[PLAYER_ID]->GetPos().x + 70, gameObjects[PLAYER_ID]->GetPos().y + 30, 0);
					}
				}
			}
			hit_stop = 20;//�q�b�g�X�g�b�v�̌p������
			slash_height = 30.0f;//�a���̍�����ݒ�
		});
}
void GameScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
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
		//Collider�̓񕪊�A�b�v�f�[�g
		DynamicAABBTree::GetInstance().Update();
		//DynamicAABBTree::GetInstance().updateDebugFlame();


		std::vector<int> toDelete;

		for (auto& pair : gameObjects)
		{
			if (pair.second)
			{
				pair.second->Update();

				// ��ʊO�ɂ������I�u�W�F�N�g�̏���
				if (pair.second->GetPos().x < playerPos.x - 100)
				{
					if (pair.second->GetObjectType() == true)
					{
						toDelete.push_back(pair.first); // �폜�\�胊�X�g�ɒǉ�
					}
				}
			}
		}

		// ���[�v�I����ɍ폜�����s
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
			gameObjects[SWORD_ID]->SetPos(playerPos.x, 300, 0);//���������Ă��Ȃ��Ƃ��͉�ʊO��
		}

		std::vector<int> ids = FindObjID("Sword");
		if (ids.size() != 0)
		{

			for (int objID : ids)
			{
				//�a����������x�̍����ɂȂ��������
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


void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
	//�r���[�ϊ��s��
	vm = camera->SetViewMatrix();
	// �v���W�F�N�V�����ϊ��s����쐬
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
	//�v���C���[�̃A�j���[�V�����`��
	gameObjects[PLAYER_ID]->Draw(vm, pm);
	//debug.DrawNode(vm, pm);

}


void GameScene::Shutdown()// �V�[���̏I�������B
{
}