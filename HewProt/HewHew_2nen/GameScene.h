#pragma once
#include "Scene.h"
#include "Raycast.h"
#include "RayCollider.h"
#include "Debug.h"
#include "GuiController.h"

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
	//std::shared_ptr<GameObject> GetGameObject(const std::string&);
private:
	Input& input = Input::GetInstance();
	Debug& debug = Debug::GetInstance();

	//斬撃のの飛ぶ高さ
	float slash_height = 0.0f;

	//ヒットストップ用
	int hit_stop = 0;
	int stop_cnt = 0;
};

