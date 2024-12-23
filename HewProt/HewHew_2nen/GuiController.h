#pragma once
#include "Raycast.h"
#include "RayCollider.h"
#include "scene.h"
#define NOSELECTED (-1)//オブジェクトを選択していない
class GuiController
{
public:
	GuiController(){}
	~GuiController() {}

	void Init(Scene* _scene, const std::string& txtName);
	void Update();
	void Draw();
	void SetGameObjects(std::unordered_map<int, std::shared_ptr<GameObject>>* _gameObject)
	{
		gameObjects = _gameObject;
	}
	void ControlGUI();
	void RenderGameObjectProperties(std::shared_ptr<GameObject>& obj);
	void RenderTextureSelector(std::shared_ptr<GameObject>& gameObject);
	void GameObjectSelector(const DirectX::XMFLOAT3& pos);
	void RenderHierarchyView();
	void RenderBottomView();
	void RenderTopView();
private:
	Scene* scene = nullptr;
	Input& input = Input::GetInstance();
	GameObjectManager& gameObjMng = GameObjectManager::GetInstance();
	TextureManager& textureManager = TextureManager::GetInstance();
	std::unordered_map<int, std::shared_ptr<GameObject>>* gameObjects=nullptr;
	std::string curentSceneName;//現在操作しているシーン名

	std::string selected_ObjectName;//選択しているオブジェクトの名前
	Raycast raycast;
	RayCollider collider;
	DirectX::XMFLOAT3 m_rayPos;//右クリック押されたときのレイののワールド座標
	int selected_ObjectID = -1;//選択しているオブジェクトのID
	bool isUsingGUI = false;//マウスカーソルがgui上にあるか
	bool drawMakeObjSelecter = false;//オブジェクト生成のUI描画フラグ

	bool runningGame=false;
};

