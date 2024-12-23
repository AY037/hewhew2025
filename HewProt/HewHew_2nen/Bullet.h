#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
class Bullet :public GameObject
{
public:
	Bullet() {
		//AddComponent("Rigidbody");　　      地面とか貫通して追いかけるために両方オフにしている。実際に使うときはコンポーネントを読み込んで
		//AddComponent("BoxCollider");　      アップデートを止める。プレイヤーに一度接触したことを確認したらフラグでコンポーネントのアップデート
		SetObjectTexName("asset/Bullet.png");//を実行するとかすると色々派生して動きを作れるかも
		SetObjTypeName("Bullet");
		//isRigidbody = true;
	}
	~Bullet() {}
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw();  //描画
private:
	const float speed = 3.0f;
	BoxCollider boxcoll;
	float shoot_angle=0.0f;
	bool flip = false;//弾の反転フラグ
};

