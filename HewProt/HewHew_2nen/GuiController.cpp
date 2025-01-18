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
			// 固定位置 (左上に表示)
			ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width, 0), ImGuiCond_Always);
			// 固定サイズ (幅300, 高さ200)
			ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
			//ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // 最小サイズ200x100, 最大800x600

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

	if ((input.GetKeyTrigger(VK_LBUTTON) == true) && isUsingGUI == false)//GUIを操作している時は実行されない
	{
		//一番近いオブジェクトIDと名前の記憶用
		std::string nearObjectName;
		int nearObjectID = NOSELECTED;
		bool selected = false;//どのオブジェクトにもレイが当たっていなかった時
		DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
			scene->GetProjectionMatrix(), (scene->GetCamera().GetCameraPos()));//マウスポインタの座標をワールド座標に変換z=0v

		//修正予定全オブジェクトとのcolliderを動かしてるのでオブジェクトが増えすぎると重い
		//----------------------------------------------------
		for (auto& pair : *gameObjects)//全オブジェクトと比較
		{
			auto& obj = pair.second;
			obj->SetColor(1.0, 1.0, 1.0, 1);//オブジェクトの色をリセット
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
			(*gameObjects)[selected_ObjectID]->SetColor(1.1, 1.1, 1.1, 1.0);//選択したオブジェクトの色を明るくする
		}

	}
	//オブジェクトのマウスポインタでの移動マウスポインタにオブジェクトが引き寄せられないように
	{
		static bool trigger_n = false;
		static bool trigger_o = false;
		if ((input.GetKeyPress(VK_LBUTTON) == true) && isUsingGUI == false)//GUIを操作している時は実行されない
		{
			if (selected_ObjectID != NOSELECTED)
			{
				trigger_o = trigger_n;
				if (trigger_n == false)
				{
					trigger_n = true;
				}
				DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
					scene->GetProjectionMatrix(), (scene->GetCamera().GetCameraPos()));//マウスポインタの座標をワールド座標に変換z=0v

				DirectX::XMVECTOR rayVec = DirectX::XMLoadFloat3(&rayPos);
				DirectX::XMFLOAT3 objPos = { 0,0,0 };
				DirectX::XMVECTOR objVec = DirectX::XMVectorZero();
				static DirectX::XMVECTOR dif = DirectX::XMVectorZero();
				if (trigger_n == true && trigger_o == false)//一回しか実行されない
				{
					objPos = (*gameObjects)[selected_ObjectID]->GetPos();
					objVec = DirectX::XMLoadFloat3(&objPos);
					dif = objVec - rayVec;//マウス座標とオブジェクトの距離を計算
				}

				DirectX::XMStoreFloat3(&objPos, rayVec + dif);//オブジェクトとの差分加算
				(*gameObjects)[selected_ObjectID]->SetPos(objPos.x, objPos.y, objPos.z);
			}
		}
		if ((input.GetKeyRelease(VK_LBUTTON) == true))//GUIを操作している時は実行されない
		{
			trigger_n = false;
		}
	}

	if ((input.GetKeyTrigger(VK_RBUTTON) == true) && isUsingGUI == false)//GUIを操作している時は実行されない
	{
		m_rayPos = raycast.Raycasting(input.GetMousePos(), scene->GetViewMatrix(),
			scene->GetProjectionMatrix(), scene->GetCamera().GetCameraPos());//マウスポインタの座標をワールド座標に変換z=0v
		drawMakeObjSelecter = true;
	}

	if (input.GetKeyPress(VK_CONTROL))
	{
		if (input.GetKeyTrigger(VK_S))
		{
			std::string textName = curentSceneName + std::string(".txt");
			scene->GetSaveLoad().SaveScene(textName, *gameObjects);//シーンの保存
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
				copyObj->Init();
				scene->AddObject(copyObj);
				scene->AddAndDelete();
			}
		}
		if (input.GetKeyTrigger(VK_Z))
		{
			//if (!staticObjects.empty()) {
			//	int lastObjID = staticObjects.size() - 1; // 最後の要素のイテレータ
			//	staticObjects.erase(lastObjID); // 削除
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
	// 固定位置 (左上に表示)
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - width, 0), ImGuiCond_Always);
	// 固定サイズ (幅300, 高さ200)
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	//ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // 最小サイズ200x100, 最大800x600

	ImGui::Begin("GameObject Properties");

	ImGui::Text("Object ID: %d", obj->GetObjID());
	ImGui::Text("Object Name: %s", obj->GetName().c_str());

	DirectX::XMFLOAT3 tmp_Pos = obj->GetPos();
	DirectX::XMFLOAT2 position = { tmp_Pos.x,tmp_Pos.y };

	DirectX::XMFLOAT3 tmp_Size = obj->GetSize();
	DirectX::XMFLOAT2 size = { tmp_Size.x,tmp_Size.y };

	// キーボード入力で直接値を変更
	if (ImGui::InputFloat2("Position", reinterpret_cast<float*>(&position))) {
		obj->SetPos(position.x, position.y, 0); // 値が変更された場合にセッター関数を呼び出す
	}

	// Size (Size)
	if (ImGui::InputFloat2("Size", reinterpret_cast<float*>(&size))) {
		obj->SetSize(size.x, size.y, 0); // 値が変更された場合にセッター関数を呼び出す
	}

	float angle = obj->GetAngle();
	// Angle (Angle)
	if (ImGui::InputFloat("Angle", reinterpret_cast<float*>(&angle))) // 角度をスライダーで変更
	{
		obj->SetAngle(angle); // 値が変更された場合にセッター関数を呼び出す
	}

	DirectX::XMFLOAT4 color = obj->GetColor();
	// Angle (Angle)
	if (ImGui::InputFloat4("Color", reinterpret_cast<float*>(&color))) // 角度をスライダーで変更
	{
		obj->SetColor(color.x, color.y, color.z, color.w); // 値が変更された場合にセッター関数を呼び出す
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
	// テクスチャ名のリストを取得
	std::vector<std::string> textureNames = textureManager.GetTextureNameList();

	// 選択中のテクスチャ名を保持する変数
	static std::string currentTextureName = "planeTexture";

	// ImGuiのドロップダウンリストを作成
	if (ImGui::BeginCombo("Select Texture", currentTextureName.c_str()))
	{
		for (const auto& textureName : textureNames)
		{
			bool isSelected = (currentTextureName == textureName); // 現在選択中か確認
			if (ImGui::Selectable(textureName.c_str(), isSelected))
			{
				currentTextureName = textureName; // 選択中のテクスチャ名を更新
				gameObject->SetTexture(textureName.c_str(), textureManager); // GameObjectにテクスチャを設定
			}

			// 現在選択されている項目にチェックをつける
			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

//void GuiController::GameObjectSelector(const XMFLOAT3& pos)
//{
//	// テクスチャ名のリストを取得
//	std::unordered_map<std::string, std::function<std::shared_ptr<GameObject>()>>& makeObjectNames = gameObjMng.GetMakeObjectList();
//
//	// 選択中のテクスチャ名を保持する変数
//	static std::string currentMakeObjectName = "planeObject";
//
//	// ImGuiのドロップダウンリストを作成
//	if (ImGui::BeginCombo("Select Object", currentMakeObjectName.c_str()))
//	{
//		for (const auto& makeObjectName : makeObjectNames)
//		{
//			bool isSelected = (currentMakeObjectName == makeObjectName.first); // 現在選択中か確認
//			if (ImGui::Selectable(makeObjectName.first.c_str(), isSelected))
//			{
//				currentMakeObjectName = makeObjectName.first; // 選択中のテクスチャ名を更新
//				std::shared_ptr<GameObject> obj = gameObjMng.GetObj(currentMakeObjectName);
//				obj->SetPos(pos.x, pos.y, 0);
//				//obj->SetSize(10, 10, 0);
//				obj->SetName(currentMakeObjectName);
//				obj->Init(textureManager);
//				scene->AddObject(obj);
//				scene->AddAndDelete();
//			}
//
//			// 現在選択されている項目にチェックをつける
//			if (isSelected)
//				ImGui::SetItemDefaultFocus();
//		}
//		ImGui::EndCombo();
//	}
//}

void GuiController::GameObjectSelector(const XMFLOAT3& pos)
{


		if (ImGui::Button("Stage", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Stage");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Stage");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("NormalEnemy", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Enemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Enemy");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		if (ImGui::Button("FlyEnemy", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("FlyEnemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("FlyEnemy");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("BulletFlyEnemy", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("BulletFlyEnemy");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("BulletFlyEnemy");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		if (ImGui::Button("Ground", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Ground");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Ground");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("DynamicStage", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("DynamicStage");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("DynamicStage");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}
	
		if (ImGui::Button("DontDragStage", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("DontDragStage");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("DontDragStage");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("FallWall", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("FallWall");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("FallWall");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		if (ImGui::Button("Coin", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Coin");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Coin");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("Goal", ImVec2(140, 60))) { // 幅200、高さ100のボタン
			std::shared_ptr<GameObject> obj = gameObjMng.GetObj("Goal");
			obj->SetPos(pos.x, pos.y, 0);
			//obj->SetSize(10, 10, 0);
			obj->SetName("Goal");
			obj->Init();
			scene->AddObject(obj);
			scene->AddAndDelete();
		}
}

void GuiController::RenderHierarchyView() {
	const int width = SCREEN_WIDTH / 6.0f;
	const int height = SCREEN_HEIGHT;

	// 固定位置とサイズ
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

	// ヒエラルキービュー用ウィンドウ
	if (ImGui::Begin("Hierarchy View", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
		// gameObjects を反復 (例として std::unordered_map<int, std::shared_ptr<GameObject>> を想定)
		for (auto& obj : *gameObjects) {
			const std::shared_ptr<GameObject>& gameObject = obj.second;
			const std::string& objName = gameObject->GetName();

			// ラベルに一意な識別子を付加
			std::string label = objName + std::to_string(gameObject->GetObjID());

			// 各オブジェクトをリスト表示
			if (ImGui::Selectable(label.c_str(), selected_ObjectID == gameObject->GetObjID())) {
				for (auto& pair : *gameObjects)//全オブジェクトと比較
				{
					auto& obj = pair.second;
					obj->SetColor(1.0, 1.0, 1.0, 1);//オブジェクトの色をリセット
				}
				// オブジェクトを選択したときに ID を更新
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

	// 固定位置とサイズ
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 6.0f, SCREEN_HEIGHT - height), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	if (ImGui::Begin("SceneView", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if (ImGui::Button("TitleScene", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("TitleScene");
			curentSceneName = "TitleScene";
			selected_ObjectID = NOSELECTED;
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("ResultScene", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("ResultScene");
			curentSceneName = "ResultScene";
			selected_ObjectID = NOSELECTED;
		}

		if (ImGui::Button("Stage1", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("Stage1");
			curentSceneName = "Stage1";
			selected_ObjectID = NOSELECTED;
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("Stage2", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("Stage2");
			curentSceneName = "Stage2";
			selected_ObjectID = NOSELECTED;
		}

		if (ImGui::Button("Stage3", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("Stage3");
			curentSceneName = "Stage3";
			selected_ObjectID = NOSELECTED;
		}

		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("Stage4", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("Stage4");
			curentSceneName = "Stage4";
			selected_ObjectID = NOSELECTED;
		}


		if (ImGui::Button("Stage5", ImVec2(100, 50))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene("Stage5");
			curentSceneName = "Stage5";
			selected_ObjectID = NOSELECTED;
		}
	}

	ImGui::End();
}

void GuiController::RenderTopView() {
	const int width = SCREEN_WIDTH / (6.0f / 4.0f);
	const int height =60.0f;
	// 固定位置とサイズ
	ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH / 6.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);
	if (ImGui::Begin("TopView", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		if(runningGame==false)
		{
			if (ImGui::Button("StartGame", ImVec2(100, 30))) { // 幅200、高さ100のボタン
				EventManager::GetInstance().SendEvent("StartGame");
				runningGame = true;
				std::string textName = curentSceneName + std::string(".txt");
				scene->GetSaveLoad().SaveScene(textName, *gameObjects);//シーンの保存
				Sound::GetInstance().Play(SOUND_LABEL_BGM001);
				selected_ObjectID = NOSELECTED;
			}
		}
		if (runningGame == true)
		{
			if (ImGui::Button("EndGame", ImVec2(100, 30))) { // 幅200、高さ100のボタン
				EventManager::GetInstance().SendEvent("EndGame");
				runningGame = false;
				selected_ObjectID = NOSELECTED;
				Sound::GetInstance().Stop(SOUND_LABEL_BGM001);
				EventManager::GetInstance().SendChangeScene(curentSceneName);
			}
		}
		ImGui::SameLine(); // 横並びに配置
		if (ImGui::Button("ResetScene", ImVec2(100, 30))) { // 幅200、高さ100のボタン
			EventManager::GetInstance().SendChangeScene(curentSceneName);
		}
		if (runningGame == false)
		{
			ImGui::SameLine(); // 横並びに配置
			if (ImGui::Button("SaveScene", ImVec2(100, 30))) {
				std::string textName = curentSceneName + std::string(".txt");
				scene->GetSaveLoad().SaveScene(textName, *gameObjects);//シーンの保存
			}
		}
	}

	ImGui::End();
}