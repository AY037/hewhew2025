#pragma once
#include "Scene.h"
#include"TextureManager.h"
#include "SaveLoad.h"
#include "Raycast.h"
#include "RayCollider.h"
#include <queue>
#include "Debug.h"
#define NOSELECTED (-1)//オブジェクトを選択していない
/// <summary>
/// ゲームの一つのシーン（ステージ、メニュー、戦闘など）を表すクラス。
/// ゲームオブジェクトの集合を管理し、そのシーンでの全体の流れを制御します。
/// </summary>
class GameScene :public Scene
{
public:
	GameScene()
	{
		camera = std::make_unique<Camera>();
		Init();
	}
	~GameScene()
	{

		// staticObjectsの解放
		for (auto& pair : gameObjects) {
			pair.second->Uninit();
		}
		gameObjects.clear(); // ベクタをクリア

		sound.Stop(SOUND_LABEL_BGM000); //BGMを停止
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
	void AddObject(std::shared_ptr<GameObject>_gameObject);
	void RenderGameObjectProperties(std::shared_ptr<GameObject>& obj);//オブジェクトのGUI描画
	void RenderTextureSelector(std::shared_ptr<GameObject>& gameObject);//オブジェクトのテクスチャリストのGUI
	void ControlGUI();
	//std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	std::unordered_map<int, std::shared_ptr<GameObject>> gameObjects; // シーン内のゲームオブジェクトリスト
	Input& input = Input::GetInstance();
	TextureManager textureManager;
	SaveLoad saveload;
	Debug& debug = Debug::GetInstance();
	std::queue<int> availableIDs;//消されたオブジェクトのIDのキューFIFO


	std::string selected_ObjectName;//選択しているオブジェクトの名前
	Raycast raycast;
	RayCollider collider;
	int selected_ObjectID = -1;//選択しているオブジェクトのID
	bool isUsingGUI = false;//マウスカーソルがgui上にあるか
	DirectX::XMMATRIX vm;
	DirectX::XMMATRIX pm;
};

