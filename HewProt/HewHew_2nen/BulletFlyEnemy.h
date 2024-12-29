#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
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
	BoxCollider boxColl;
	int shoot_cnt = 0;//bullet生成用フレームカウント
	const int shoot_time = 120;//Bullet生成のレート低くすれば速くなる

	float move_angle = 0.0f;
	int firecnt = 0;//何回玉を撃ったか。3回撃ったら終わり

	bool attack_fg = false;//3回玉をうつとアタックする
	bool hit_attack = false;//当たったかどうか

};

