#pragma once
#include "Scene.h"
#include"TextureManager.h"
/// <summary>
/// ゲームの一つのシーン（ステージ、メニュー、戦闘など）を表すクラス。
/// ゲームオブジェクトの集合を管理し、そのシーンでの全体の流れを制御します。
/// </summary>
class GameScene:public Scene
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
		for (auto obj : staticObjects) {
			obj->Uninit();
		}
		staticObjects.clear(); // ベクタをクリア

		// dynamicObjectsの解放
		for (auto& pair : dynamicObjects) {
			for (auto& obj : pair.second) {
				obj->Uninit();
			}
		}
		dynamicObjects.clear(); // ベクタをクリア

		sound.Stop(SOUND_LABEL_BGM000); //BGMを停止
	}
	void Init();	 // シーンの初期化。ここにオブジェクトを追加
	void Update();	 // シーン内のオブジェクト更新。
	void Draw();	 // シーン内のオブジェクトを描画
	void Shutdown(); // シーンの終了処理。
	void AddObject(const std::string& objectName, std::shared_ptr<GameObject> _gameObject);
	//void collider(GameObject&);//後でコンポーネント化
	std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	std::vector<std::shared_ptr<GameObject>> staticObjects; // シーン内のゲームオブジェクトリスト
	std::unordered_map<std::string, std::list<std::shared_ptr<GameObject>>> dynamicObjects; // 動的オブジェクトの管理
	Input& input = Input::GetInstance();
	TextureManager textureManager;
};

