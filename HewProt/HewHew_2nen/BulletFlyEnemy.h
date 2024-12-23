#pragma once
#include "GameObject.h"
class BulletFlyEnemy :public GameObject
{
public:
	BulletFlyEnemy() {
		AddComponent("Rigidbody");
		AddComponent("BoxCollider");
		SetObjectTexName("asset/FlyEnemy.png");
		SetName("BulletFlyEnemy");
		SetObjTypeName("Enemy");//オブジェクトの属性エネミー共通の処理で使う用に
		isRigidbody = true;//静的か動的か
	}
	~BulletFlyEnemy() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	int shoot_cnt = 0;//bullet生成用フレームカウント
	const int shoot_time = 120;//Bullet生成のレート低くすれば速くなる
};

