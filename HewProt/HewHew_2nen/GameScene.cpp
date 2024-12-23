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

//�I�u�W�F�N�gID
//#define PLAYER_ID (3)
//#define SWORD_ID (4)

using namespace DirectX;

void GameScene::Init()// �V�[���̏������B
{
	camera.Init();

	//�V�[���̃��[�h
	std::string txtName = sceneName + std::string(".txt");
	saveload.LoadScene(txtName, gameObjects, gameObjectList, textureManager);

	for (auto& obj : gameObjectList)
	{
		if ((*obj)) (*obj)->Init(textureManager);
	}

	for (auto& obj : gameObjectList)
	{
		if ((*obj)->GetName() == "Player") PLAYER_ID = (*obj)->GetObjID();
		if ((*obj)->GetName() == "DragSword") DRAGSWORD_ID = (*obj)->GetObjID();
	}

	int size = GameManager::GetInstance().GetPlayerHP();
	for (int i = 1; i <= size; ++i)
	{
		std::shared_ptr<UI> heart=std::make_shared<UI>();
		heart->SetSize(SCREEN_WIDTH / 20, SCREEN_WIDTH / 20, 0);
		heart->SetPos(-SCREEN_WIDTH / 2 + SCREEN_WIDTH / 20 * i, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 20 * 3), 0);
		heart->Initialize("asset/Heart.png",TextureManager::GetInstance());
		uiObjectList.push_back(heart);
	}

	//Collieder�񕪖؏�����
	DynamicAABBTree::GetInstance().Init(gameObjects);
	DynamicAABBTree::GetInstance().SetLeaf();

#ifdef GUI_MODE
	Engine::GetInstance().GetGuiController().Init(this, sceneName);
#endif

	SetEventManager();
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
		camera.Update(playerPos, false);
		hit_stop = 0;
		stop_cnt = 0;
		//Collider�̓񕪊�A�b�v�f�[�g
		DynamicAABBTree::GetInstance().Update();
		//DynamicAABBTree::GetInstance().updateDebugFlame();

		for (auto& obj : gameObjectList)
		{
			if ((*obj))
			{
				if ((*obj)->GetPos().x > playerPos.x - 50&& (*obj)->GetPos().x < playerPos.x + 300)
				{
					(*obj)->Update();
				}

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

		//�I�u�W�F�N�g�̒ǉ��Ə���
		AddAndDelete();
	}
	else
	{
		camera.Update(playerPos, true);
	}
	//�v���C���[��hp�Ɖ�ʏ�̃n�[�g�̐������킹��
	if (uiObjectList.size() != GameManager::GetInstance().GetPlayerHP())
	{
		uiObjectList.erase(uiObjectList.begin() + uiObjectList.size()-1);
	}
}


void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
	//�r���[�ϊ��s��
	vm = camera.SetViewMatrix();
	// �v���W�F�N�V�����ϊ��s����쐬
	pm = camera.SetProjectionMatrix();

	for (auto& obj : gameObjectList)
	{
		if ((*obj)) {
			(*obj)->DrawObject(vm, pm);

		}
	}
	//�v���C���[�̃A�j���[�V�����`��
	gameObjects[PLAYER_ID]->Draw(vm, pm);

	//UI�̕`��
	for (auto& obj : uiObjectList)
	{
		if (obj) {
			obj->DrawUiObject(pm);
		}
	}

}


void GameScene::Shutdown()// �V�[���̏I�������B
{
	//�񕪖؂̃��Z�b�g
	DynamicAABBTree::GetInstance().reset();
}


void GameScene::SetEventManager()
{
	EventManager::GetInstance().SetObjectIdFunc("Shoot", [this]( const int objId) {
		std::shared_ptr<GameObject> obj = GameObjectManager::GetInstance().GetObj("Bullet");
		DirectX::XMFLOAT3 pos = gameObjects[objId]->GetPos();
		obj->SetPos(pos.x,pos.y,pos.z);
		this->AddObject(obj);
		});

	EventManager::GetInstance().SetObjectIdFunc("Delete", [this]( const int objId) {AddRemoveObject(objId); });

	EventManager::GetInstance().SetObjectIdFunc("TransDebri", [this](const int objId) {
	gameObjects[objId]->SetSize(7.0f,7.0f,0.0f);
	gameObjects[objId]->SetName("Debri");
	gameObjects[objId]->SetObjTypeName("Debri"); 
	gameObjects[objId]->SetIsBoxColl(true);
		});


	//�������蔻��̏���
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
				DirectX::XMFLOAT3 velocity = { 4.0f, 1.1f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 16);
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
				DirectX::XMFLOAT3 velocity = { 4.5f, 1.4f, 0.0f };
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 25);
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
						//gameObjects[objID]->SetPos(pos.x + 10, pos.y + 5, 0);
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
				std::shared_ptr<GameObject> obj = std::make_shared<Sword>(velocity, 25);
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
						//gameObjects[objID]->SetPos(pos.x + 10, pos.y + 5, 0);
						gameObjects[objID]->SetVelocity(velocity);
					}
				}
			}
			hit_stop = 20;//�q�b�g�X�g�b�v�̌p������
		});
}