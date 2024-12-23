#include "GuiController.h"
#include "imgui.h"
#include "EventManager.h"
using namespace DirectX;
void GuiController::Init(Scene* _scene,const std::string& txtName)
{
	scene = _scene;
	gameObjects = scene->GetGameObjects();
	textureManager = scene->GetTextureManager();
	curentSceneName = txtName;
}

void GuiController::Update()
{
	ControlGUI();
}

void GuiController::Draw() {
	RenderTopView();
	if(!runningGame)
	{
		RenderHierarchyView();
		RenderBottomView();
		if (selected_ObjectID != NOSELECTED)
		{
			auto& obj = (*gameObjects)[selected_ObjectID];
			if (obj)
			{
				RenderGameObjectProperties(obj);
			}
		}
		else
		{
			const int width = SCREEN_WIDTH / 6.0f;
			const int height = SCREEN_HEIGHT;
			// �Œ�ʒu (����ɕ\��)
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width, 0), ImGuiCond_Always);
			// �Œ�T�C�Y (��300, ����200)
			ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
			//ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // �ŏ��T�C�Y200x100, �ő�800x600

			ImGui::Begin("GameObject Properties");

			ImGui::Text("NOSELECTED");
			if (drawMakeObjSelecter == true)
			{
				GameObjectSelector(m_rayPos);
			}

			ImGui::End();
		}
	}
}

void GuiController::ControlGUI()
{
	if (ImGui::IsWindowHovered() == true || ImGui::IsAnyItemHovered() == true)
	{
		isUsingGUI = true;
	}
	else
	{
		isUsingGUI = false;
	}

	if ((input.GetKeyTrigger(VK_LBUTTON) == true) && isUsingGUI == false)//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
	{
		//��ԋ߂��I�u�W�F�N�gID�Ɩ��O�̋L���p
		std::string nearObjectName;
		int nearObjectID = NOSELECTED;
		bool selected = false;//�ǂ̃I�u�W�F�N�g�ɂ����C���������Ă��Ȃ�������
		DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
			scene->GetProjectionMatrix(), (scene->GetCamera().GetCameraPos()));//�}�E�X�|�C���^�̍��W�����[���h���W�ɕϊ�z=0v

		//�C���\��S�I�u�W�F�N�g�Ƃ�collider�𓮂����Ă�̂ŃI�u�W�F�N�g������������Əd��
		//----------------------------------------------------
		for (auto& pair : *gameObjects)//�S�I�u�W�F�N�g�Ɣ�r
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
			drawMakeObjSelecter = false;
		}
		else
		{
			selected_ObjectName = nearObjectName;
			selected_ObjectID = nearObjectID;
			(*gameObjects)[selected_ObjectID]->SetColor(1.1, 1.1, 1.1, 1.0);//�I�������I�u�W�F�N�g�̐F�𖾂邭����
		}

	}
	//�I�u�W�F�N�g�̃}�E�X�|�C���^�ł̈ړ��}�E�X�|�C���^�ɃI�u�W�F�N�g�������񂹂��Ȃ��悤��
	{
		static bool trigger_n = false;
		static bool trigger_o = false;
		if ((input.GetKeyPress(VK_LBUTTON) == true) && isUsingGUI == false)//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
		{
			if (selected_ObjectID != NOSELECTED)
			{
				trigger_o = trigger_n;
				if (trigger_n == false)
				{
					trigger_n = true;
				}
				DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
					scene->GetProjectionMatrix(), (scene->GetCamera().GetCameraPos()));//�}�E�X�|�C���^�̍��W�����[���h���W�ɕϊ�z=0v

				DirectX::XMVECTOR rayVec = DirectX::XMLoadFloat3(&rayPos);
				DirectX::XMFLOAT3 objPos = { 0,0,0 };
				DirectX::XMVECTOR objVec = DirectX::XMVectorZero();
				static DirectX::XMVECTOR dif = DirectX::XMVectorZero();
				if (trigger_n == true && trigger_o == false)//��񂵂����s����Ȃ�
				{
					objPos = (*gameObjects)[selected_ObjectID]->GetPos();
					objVec = DirectX::XMLoadFloat3(&objPos);
					dif = objVec - rayVec;//�}�E�X���W�ƃI�u�W�F�N�g�̋������v�Z
				}

				DirectX::XMStoreFloat3(&objPos, rayVec + dif);//�I�u�W�F�N�g�Ƃ̍������Z
				(*gameObjects)[selected_ObjectID]->SetPos(objPos.x, objPos.y, objPos.z);
			}
		}
		if ((input.GetKeyRelease(VK_LBUTTON) == true))//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
		{
			trigger_n = false;
		}
	}

	if ((input.GetKeyTrigger(VK_RBUTTON) == true) && isUsingGUI == false)//GUI�𑀍삵�Ă��鎞�͎��s����Ȃ�
	{
		m_rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
			scene->GetProjectionMatrix(), scene->GetCamera().GetCameraPos());//�}�E�X�|�C���^�̍��W�����[���h���W�ɕϊ�z=0v
		drawMakeObjSelecter = true;
	}

	if (input.GetKeyPress(VK_CONTROL))
	{
		if (input.GetKeyTrigger(VK_S))
		{
			std::string textName = curentSceneName + std::string(".txt");
			scene->GetSaveLoad().SaveScene(textName, *gameObjects);//�V�[���̕ۑ�
		}

		if (input.GetKeyTrigger(VK_C))
		{
		}

		if (input.GetKeyTrigger(VK_V))
		{
			if (selected_ObjectID != NOSELECTED)
			{
				std::shared_ptr<GameObject>  copyObj = GameObjectManager::GetInstance().GetObj(selected_ObjectName);
				copyObj->SetName(selected_ObjectName);
				copyObj->SetObjectTexName((*gameObjects)[selected_ObjectID]->GetObjectTexName());
				DirectX::XMFLOAT3 pos = (*gameObjects)[selected_ObjectID]->GetPos();
				DirectX::XMFLOAT3 size = (*gameObjects)[selected_ObjectID]->GetSize();
				copyObj->SetPos(pos.x + 11, pos.y, pos.z);
				copyObj->SetSize(size.x, size.y, size.z);
				copyObj->SetAngle((*gameObjects)[selected_ObjectID]->GetAngle());
				copyObj->Init(textureManager);
				scene->AddObject(copyObj);
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
			scene->DeleteObject(selected_ObjectID);
			selected_ObjectID = NOSELECTED;
		}
	}
}

void GuiController::RenderGameObjectProperties(std::shared_ptr<GameObject>& obj)
{
	const int width = SCREEN_WIDTH / 6.0f;
	const int height = SCREEN_HEIGHT;
	// �Œ�ʒu (����ɕ\��)
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width, 0), ImGuiCond_Always);
	// �Œ�T�C�Y (��300, ����200)
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	//ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // �ŏ��T�C�Y200x100, �ő�800x600

	ImGui::Begin("GameObject Properties");

	ImGui::Text("Object ID: %d", obj->GetObjID());
	ImGui::Text("Object Name: %s", obj->GetName().c_str());

	DirectX::XMFLOAT3 tmp_Pos = obj->GetPos();
	DirectX::XMFLOAT2 position = { tmp_Pos.x,tmp_Pos.y };

	DirectX::XMFLOAT3 tmp_Size = obj->GetSize();
	DirectX::XMFLOAT2 size = { tmp_Size.x,tmp_Size.y };

	// �L�[�{�[�h���͂Œ��ڒl��ύX
	if (ImGui::InputFloat2("Position", reinterpret_cast<float*>(&position))) {
		obj->SetPos(position.x, position.y, 0); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
	}

	// Size (Size)
	if (ImGui::InputFloat2("Size", reinterpret_cast<float*>(&size))) {
		obj->SetSize(size.x, size.y, 0); // �l���ύX���ꂽ�ꍇ�ɃZ�b�^�[�֐����Ăяo��
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

	if (drawMakeObjSelecter == true)
	{
		GameObjectSelector(m_rayPos);
	}

	ImGui::End();

}

void GuiController::RenderTextureSelector(std::shared_ptr<GameObject>& gameObject)
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
}

//void GuiController::GameObjectSelector(const XMFLOAT3& pos)
//{
//	// �e�N�X�`�����̃��X�g���擾
//	std::unordered_map<std::string, std::function<std::shared_ptr<GameObject>()>>& makeObjectNames = gameObjMng.GetMakeObjectList();
//
//	// �I�𒆂̃e�N�X�`������ێ�����ϐ�
//	static std::string currentMakeObjectName = "planeObject";
//
//	// ImGui�̃h���b�v�_�E�����X�g���쐬
//	if (ImGui::BeginCombo("Select Object", currentMakeObjectName.c_str()))
//	{
//		for (const auto& makeObjectName : makeObjectNames)
//		{
//			bool isSelected = (currentMakeObjectName == makeObjectName.first); // ���ݑI�𒆂��m�F
//			if (ImGui::Selectable(makeObjectName.first.c_str(), isSelected))
//			{
//				currentMakeObjectName = makeObjectName.first; // �I�𒆂̃e�N�X�`�������X�V
//				std::shared_ptr<GameObject> obj = gameObjMng.GetObj(currentMakeObjectName);
//				obj->SetPos(pos.x, pos.y, 0);
//				//obj->SetSize(10, 10, 0);
//				obj->SetName(currentMakeObjectName);
//				obj->Init(textureManager);
//				scene->AddObject(obj);
//				scene->AddAndDelete();
//			}
//
//			// ���ݑI������Ă��鍀�ڂɃ`�F�b�N������
//			if (isSelected)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//}

void GuiController::GameObjectSelector(const XMFLOAT3& pos)
{


		if (ImGui::Button("Stage", ImVec2(140, 60))) { // ��200�A����100�̃{�^��
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Stage");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Stage");
			obj->Init(textureManager);
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("NormalEnemy", ImVec2(140, 60))) { // ��200�A����100�̃{�^��
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Enemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Enemy");
			obj->Init(textureManager);
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		if (ImGui::Button("FlyEnemy", ImVec2(140, 60))) { // ��200�A����100�̃{�^��
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("FlyEnemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("FlyEnemy");
			obj->Init(textureManager);
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("BulletFlyEnemy", ImVec2(140, 60))) { // ��200�A����100�̃{�^��
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("BulletFlyEnemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("BulletFlyEnemy");
			obj->Init(textureManager);
			scene->AddObject(obj);
			scene->AddAndDelete();
		}
	
}

void GuiController::RenderHierarchyView() {
	const int width = SCREEN_WIDTH / 6.0f;
	const int height = SCREEN_HEIGHT;

	// �Œ�ʒu�ƃT�C�Y
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

	// �q�G�����L�[�r���[�p�E�B���h�E
	if (ImGui::Begin("Hierarchy View", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		// gameObjects �𔽕� (��Ƃ��� std::unordered_map<int, std::shared_ptr<GameObject>> ��z��)
		for (auto& obj : *gameObjects) {
			const std::shared_ptr<GameObject>& gameObject = obj.second;
			const std::string& objName = gameObject->GetName();

			// ���x���Ɉ�ӂȎ��ʎq��t��
			std::string label = objName + std::to_string(gameObject->GetObjID());

			// �e�I�u�W�F�N�g�����X�g�\��
			if (ImGui::Selectable(label.c_str(), selected_ObjectID == gameObject->GetObjID())) {
				for (auto& pair : *gameObjects)//�S�I�u�W�F�N�g�Ɣ�r
				{
					auto& obj = pair.second;
					obj->SetColor(1.0, 1.0, 1.0, 1);//�I�u�W�F�N�g�̐F�����Z�b�g
				}
				// �I�u�W�F�N�g��I�������Ƃ��� ID ���X�V
				selected_ObjectID = gameObject->GetObjID();
				gameObject->SetColor(1.1, 1.1, 1.1, 1.0);
			}
		}
	}
	ImGui::End();
}

void GuiController::RenderBottomView() {
	const int width = SCREEN_WIDTH / (6.0f / 4.0f);
	const int height = SCREEN_HEIGHT / 3;

	// �Œ�ʒu�ƃT�C�Y
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 6.0f, SCREEN_HEIGHT - height), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	if (ImGui::Begin("SceneView", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if (ImGui::Button("TitleScene", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("TitleScene");
			curentSceneName = "TitleScene";
		}

		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("ResultScene", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("ResultScene");
			curentSceneName = "ResultScene";
		}

		if (ImGui::Button("Stage1", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("Stage1");
			curentSceneName = "Stage1";
		}

		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("Stage2", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("Stage2");
			curentSceneName = "Stage2";
		}

		if (ImGui::Button("Stage3", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("Stage3");
			curentSceneName = "Stage3";
		}

		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("Stage4", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("Stage4");
			curentSceneName = "Stage4";
		}


		if (ImGui::Button("Stage5", ImVec2(100, 50))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene("Stage5");
			curentSceneName = "Stage5";
		}
	}

	ImGui::End();
}

void GuiController::RenderTopView() {
	const int width = SCREEN_WIDTH / (6.0f / 4.0f);
	const int height =60.0f;
	// �Œ�ʒu�ƃT�C�Y
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 6.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	if (ImGui::Begin("TopView", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if(runningGame==false)
		{
			if (ImGui::Button("StartGame", ImVec2(100, 30))) { // ��200�A����100�̃{�^��
				EventManager::GetInstance().SendEvent("StartGame");
				runningGame = true;
				std::string textName = curentSceneName + std::string(".txt");
				scene->GetSaveLoad().SaveScene(textName, *gameObjects);//�V�[���̕ۑ�
				Sound::GetInstance().Play(SOUND_LABEL_BGM001);
			}
		}
		if (runningGame == true)
		{
			if (ImGui::Button("EndGame", ImVec2(100, 30))) { // ��200�A����100�̃{�^��
				EventManager::GetInstance().SendEvent("EndGame");
				runningGame = false;
				selected_ObjectID = NOSELECTED;
				Sound::GetInstance().Stop(SOUND_LABEL_BGM001);
				EventManager::GetInstance().SendChangeScene(curentSceneName);
			}
		}
		ImGui::SameLine(); // �����тɔz�u
		if (ImGui::Button("ResetScene", ImVec2(100, 30))) { // ��200�A����100�̃{�^��
			EventManager::GetInstance().SendChangeScene(curentSceneName);
		}
	}

	ImGui::End();
}