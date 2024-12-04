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
void GameScene::Init()// シーンの初期化。
{
	camera->Init();
	//sound.Play(SOUND_LABEL_BGM000); //BGMを再生定

	//オブジェクトの生成
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
void GameScene::Update()// シーン内のオブジェクト更新。
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
		// 動的オブジェクトの更新
		for (auto& gameObject : pair.second) {
			gameObject->Update();
		}
	}

	DynamicAABBTree::GetInstance().Update();
	//DynamicAABBTree::GetInstance().updateDebugFlame();
#ifdef GUI_MODE
	if ((input.GetKeyTrigger(VK_LBUTTON) == true) && isUsingGUI == false)//GUIを操作している時は実行されない
	{
		//一番近いオブジェクトIDと名前の記憶用
		std::string nearObjectName;
		int nearObjectID = NOSELECTED;
		bool selected = false;//どのオブジェクトにもレイが当たっていなかった時
		DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), vm, pm, camera->GetCameraPos());//マウスポインタの座標をワールド座標に変換z=0

		//修正予定全オブジェクトとのcolliderを動かしてるのでオブジェクトが増えすぎると重い
		//----------------------------------------------------
		for (auto& pair : staticObjects)//全オブジェクトと比較
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
		}
		else
		{
			selected_ObjectName = nearObjectName;
			selected_ObjectID = nearObjectID;
			staticObjects[selected_ObjectID]->SetColor(1.1, 1.1, 1.1, 1.0);//選択したオブジェクトの色を明るくする
		}

	}
	//オブジェクトのマウスポインタでの移動マウスポインタにオブジェクトが引き寄せられないように
	{
		static bool trigger_n = false;
		static bool trigger_o = false;
		if ((input.GetKeyPress(VK_LBUTTON) == true) && isUsingGUI == false)//GUIを操作している時は実行されない
		{
			if(selected_ObjectID != NOSELECTED)
			{
				trigger_o = trigger_n;
				if (trigger_n == false)
				{
					trigger_n = true;
				}
				DirectX::XMFLOAT3 rayPos = raycast.Raycasting(input.GetMousePos(), vm, pm, camera->GetCameraPos());//マウスポインタの座標をワールド座標に変換z=0v
				DirectX::XMVECTOR rayVec = DirectX::XMLoadFloat3(&rayPos);
				DirectX::XMFLOAT3 objPos = { 0,0,0 };
				DirectX::XMVECTOR objVec = DirectX::XMVectorZero();
				static DirectX::XMVECTOR dif = DirectX::XMVectorZero();
				if (trigger_n == true && trigger_o == false)//一回しか実行されない
				{
					objPos = staticObjects[selected_ObjectID]->GetPos();
					objVec = DirectX::XMLoadFloat3(&objPos);
					dif = objVec - rayVec;//マウス座標とオブジェクトの距離を計算
				}

				DirectX::XMStoreFloat3(&objPos, rayVec + dif);//オブジェクトとの差分加算
				staticObjects[selected_ObjectID]->SetPos(objPos.x, objPos.y, objPos.z);
			}
		}
		if ((input.GetKeyRelease(VK_LBUTTON) == true))//GUIを操作している時は実行されない
		{
			trigger_n = false;
		}
	}

	if (input.GetKeyPress(VK_CONTROL))
	{
		if (input.GetKeyTrigger(VK_S))
		{
			textStream.SaveScene("scene1.txt", staticObjects);//シーンの保存
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
				staticObjects[id] = copyObj;//リストの最後尾にコピー
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
			staticObjects.erase(selected_ObjectID); // 削除
			availableIDs.push(selected_ObjectID);
			selected_ObjectID = NOSELECTED;
		}
	}
#endif
	camera->Update();
}
void GameScene::Draw()// シーン内のオブジェクトを描画
{
	//ビュー変換行列
	vm = camera->SetViewMatrix();
	// プロジェクション変換行列を作成
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
void GameScene::Shutdown()// シーンの終了処理。
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
	ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(800, 600)); // 最小サイズ200x100, 最大800x600
	DirectX::XMFLOAT3 position = obj->GetPos();
	// キーボード入力で直接値を変更
	if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&position))) {
		obj->SetPos(position.x, position.y, position.z); // 値が変更された場合にセッター関数を呼び出す
	}

	DirectX::XMFLOAT3 size = obj->GetSize();
	// Size (Size)
	if (ImGui::InputFloat3("Size", reinterpret_cast<float*>(&size))) {
		obj->SetSize(size.x, size.y, size.z); // 値が変更された場合にセッター関数を呼び出す
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
;
}

