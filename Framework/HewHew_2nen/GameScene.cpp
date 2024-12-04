#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "EventManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "Math.h"
void GameScene::Init()// �V�[���̏������B
{
	camera->Init();
	//sound.Play(SOUND_LABEL_BGM000); //BGM���Đ���

	//�I�u�W�F�N�g�̐���
	//staticObjects.push_back(std::make_shared<Background>());
	//AddObject("Player", std::make_shared<Player>());

	textStream.LoadScene("scene3.txt", staticObjects);

	for (auto& obj : staticObjects)
	{
		if (obj.second) obj.second->Init(textureManager);
	}

	DynamicAABBTree::GetInstance().Init(staticObjects);
	DynamicAABBTree::GetInstance().SetLeaf();
	//EventManager::GetInstance().AddListener("shoot", [this]() {
	//    this->AddObject("",new Bullet(textureManager, GetGameObject("Player")->GetPos())); });
 /*   EventManager::GetInstance().AddListener("shoot", [this]() {
		this->AddObject("Bullet", std::make_shared<Bullet>(textureManager, camera->GetCameraPos(), camera->GetCameraAngle())); });*/
}
void GameScene::Update()// �V�[�����̃I�u�W�F�N�g�X�V�B
{
	for (auto& pair : staticObjects)
	{
		if (pair.second)
		{
			pair.second->Update();
		}
	}

	for (auto& pair : dynamicObjects)
	{
		// ���I�I�u�W�F�N�g�̍X�V
		for (auto& gameObject : pair.second) {
			gameObject->Update();
		}
	}

	DynamicAABBTree::GetInstance().Update();
	//DynamicAABBTree::GetInstance().updateDebugFlame();
#ifdef GUI_MODE
	if ((input.GetKeyTrigger(VK_LBUTTON) == true) && isUsingGUI == false)//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
	{
		//��ԋ߂��I�u�W�F�N�gID�Ɩ��O�̋L���p
		std::string nearObjectName;
		int nearObjectID = NOSELECTED;
		bool selected = false;//�ǂ̃I�u�W�F�N�g�ɂ����C���������Ă��Ȃ�������
		DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), vm, pm, camera->GetCameraPos());//�}�E�X�|�C���^�̍��W�����[���h���W�ɕϊ�z=0

		//�C���\��S�I�u�W�F�N�g�Ƃ�collider�𓮂����Ă�̂ŃI�u�W�F�N�g������������Əd��
		//----------------------------------------------------
		for (auto& pair : staticObjects)//�S�I�u�W�F�N�g�Ɣ�r
		{
			auto& obj = pair.second;
			obj->SetColor(1.0, 1.0, 1.0, 1);//�I�u�W�F�N�g�̐F�����Z�b�g
			if (collider.RayBoxCollider(rayPos, obj) == true)
			{
				nearObjectName = obj->GetName();
				nearObjectID = pair.first;
				selected = true;
			}
		}
		//----------------------------------------------------

		if (selected == false)
		{
			nearObjectID = NOSELECTED;
			selected_ObjectID = NOSELECTED;
		}
		else
		{
			selected_ObjectName = nearObjectName;
			selected_ObjectID = nearObjectID;
			staticObjects[selected_ObjectID]->SetColor(1.1, 1.1, 1.1, 1.0);//�I�������I�u�W�F�N�g�̐F�𖾂邭����
		}

	}
	//�I�u�W�F�N�g�̃}�E�X�|�C���^�ł̈ړ��}�E�X�|�C���^�ɃI�u�W�F�N�g�������񂹂��Ȃ��悤��
	{
		static bool trigger_n = false;
		static bool trigger_o = false;
		if ((input.GetKeyPress(VK_LBUTTON) == true) && isUsingGUI == false)//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
		{
			if(selected_ObjectID != NOSELECTED)
			{
				trigger_o = trigger_n;
				if (trigger_n == false)
				{
					trigger_n = true;
				}
				DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), vm, pm, camera->GetCameraPos());//�}�E�X�|�C���^�̍��W�����[���h���W�ɕϊ�z=0v
				DirectX::XMVECTOR rayVec = DirectX::XMLoadFloat3(&rayPos);
				DirectX::XMFLOAT3 objPos = { 0,0,0 };
				DirectX::XMVECTOR objVec = DirectX::XMVectorZero();
				static DirectX::XMVECTOR dif = DirectX::XMVectorZero();
				if (trigger_n == true && trigger_o == false)//��񂵂����s����Ȃ�
				{
					objPos = staticObjects[selected_ObjectID]->GetPos();
					objVec = DirectX::XMLoadFloat3(&objPos);
					dif = objVec - rayVec;//�}�E�X���W�ƃI�u�W�F�N�g�̋������v�Z
				}

				DirectX::XMStoreFloat3(&objPos, rayVec + dif);//�I�u�W�F�N�g�Ƃ̍������Z
				staticObjects[selected_ObjectID]->SetPos(objPos.x, objPos.y, objPos.z);
			}
		}
		if ((input.GetKeyRelease(VK_LBUTTON) == true))//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
		{
			trigger_n = false;
		}
	}

	if (input.GetKeyPress(VK_CONTROL))
	{
		if (input.GetKeyTrigger(VK_S))
		{
			textStream.SaveScene("scene1.txt", staticObjects);//�V�[���̕ۑ�
		}

		if (input.GetKeyTrigger(VK_C))
		{
		}

		if (input.GetKeyTrigger(VK_V))
		{
			if (selected_ObjectID != NOSELECTED)
			{
				std::shared_ptr<GameObject>  copyObj = GameObjectManager::GetInstance().GetObj(selected_ObjectName);
				copyObj->Init(textureManager);
				copyObj->SetName(selected_ObjectName);
				DirectX::XMFLOAT3 pos = staticObjects[selected_ObjectID]->GetPos();
				DirectX::XMFLOAT3 size = staticObjects[selected_ObjectID]->GetSize();
				copyObj->SetPos(pos.x + 10, pos.y, pos.z);
				copyObj->SetSize(size.x, size.y, size.z);
				copyObj->SetAngle(staticObjects[selected_ObjectID]->GetAngle());
				int id = staticObjects.size();
				if (!availableIDs.empty()) {
					id = availableIDs.front();
					availableIDs.pop();
				}
				staticObjects[id] = copyObj;//���X�g�̍Ō���ɃR�s�[
			}
		}
		if (input.GetKeyTrigger(VK_Z))
		{
			//if (!staticObjects.empty()) {
			//	int lastObjID = staticObjects.size() - 1; // �Ō�̗v�f�̃C�e���[�^
			//	staticObjects.erase(lastObjID); // �폜
			//	selected_ObjectID = NOSELECTED;
			//}
		}
	}
	if (input.GetKeyPress(VK_DELETE))
	{
		if (selected_ObjectID != NOSELECTED)
		{
			staticObjects.erase(selected_ObjectID); // �폜
			availableIDs.push(selected_ObjectID);
			selected_ObjectID = NOSELECTED;
		}
	}
#endif
	camera->Update();
}
void GameScene::Draw()// �V�[�����̃I�u�W�F�N�g��`��
{
	//�r���[�ϊ��s��
	vm = camera->SetViewMatrix();
	// �v���W�F�N�V�����ϊ��s����쐬
	pm = camera->SetProjectionMatrix();

	for (auto& pair : staticObjects)
	{
		if (pair.second) pair.second->DrawObject(vm, pm);
	}

	debug.DrawNode(vm, pm);

#ifdef GUI_MODE
	if (selected_ObjectID != NOSELECTED)
	{
		auto& obj = staticObjects[selected_ObjectID];
		if (obj)
		{
			RenderGameObjectProperties(obj);
		}
	}
#endif

	for (auto& pair : dynamicObjects) {
		for (auto& gameObject : pair.second) {
			gameObject->DrawObject(vm, pm);
		}
	}

}
void GameScene::Shutdown()// �V�[���̏I�������B
{
}

void GameScene::AddObject(const std::string& objectName, std::shared_ptr<GameObject> _gameObject)
{
	if (_gameObject) {
		dynamicObjects[objectName].push_back(_gameObject);
	}
}

std::shared_ptr<GameObject> GameScene::GetGameObject(const std::string& objectName)
{
	if (dynamicObjects.find(objectName) != dynamicObjects.end()) {
		for (auto dynamicObject : dynamicObjects[objectName]) {
			return dynamicObject;
		}
	}
}

void GameScene::RenderGameObjectProperties(std::shared_ptr<GameObject>& obj)
{
	ImGui::Begin("GameObject Properties");
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // �ŏ��T�C�Y200x100, �ő�800x600
	DirectX::XMFLOAT3 position = obj->GetPos();
	// �L�[�{�[�h���͂Œ��ڒl��ύX
	if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&position))) {
		obj->SetPos(position.x, position.y, position.z); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
	}

	DirectX::XMFLOAT3 size = obj->GetSize();
	// Size (Size)
	if (ImGui::InputFloat3("Size", reinterpret_cast<float*>(&size))) {
		obj->SetSize(size.x, size.y, size.z); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
	}

	float angle = obj->GetAngle();
	// Angle (Angle)
	if (ImGui::InputFloat("Angle", reinterpret_cast<float*>(&angle))) // �p�x���X���C�_�[�ŕύX
	{
		obj->SetAngle(angle); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
	}

	DirectX::XMFLOAT4 color = obj->GetColor();
	// Angle (Angle)
	if (ImGui::InputFloat4("Color", reinterpret_cast<float*>(&color))) // �p�x���X���C�_�[�ŕύX
	{
		obj->SetColor(color.x, color.y, color.z, color.w); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
	}


	RenderTextureSelector(obj);

	//bool hovered = ImGui::IsWindowHovered();
	//ImGui::Text("Is Hovered: %s", hovered ? "Yes" : "No");

	if (ImGui::IsWindowHovered() == true || ImGui::IsAnyItemHovered() == true)
	{
		isUsingGUI = true;
	}
	else
	{
		isUsingGUI = false;
	}

	ImGui::End();

}

void GameScene::RenderTextureSelector(std::shared_ptr<GameObject>& gameObject)
{
	// �e�N�X�`�����̃��X�g���擾
	std::vector<std::string> textureNames = textureManager.GetTextureNameList();

	// �I�𒆂̃e�N�X�`������ێ�����ϐ�
	static std::string currentTextureName = "planeTexture";

	// ImGui�̃h���b�v�_�E�����X�g���쐬
	if (ImGui::BeginCombo("Select Texture", currentTextureName.c_str()))
	{
		for (const auto& textureName : textureNames)
		{
			bool isSelected = (currentTextureName == textureName); // ���ݑI�𒆂��m�F
			if (ImGui::Selectable(textureName.c_str(), isSelected))
			{
				currentTextureName = textureName; // �I�𒆂̃e�N�X�`�������X�V
				gameObject->SetTexture(textureName.c_str(), textureManager); // GameObject�Ƀe�N�X�`����ݒ�
			}

			// ���ݑI������Ă��鍀�ڂɃ`�F�b�N������
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
;
}

