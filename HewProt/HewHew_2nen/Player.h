#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
enum AnimationState
{
	ATTACK_ANI,		//通常攻撃
	BIG_ATTACK_ANI1,//ため攻撃
	BIG_ATTACK_ANI2,//強ため攻撃
	DRAG_ANI		//引きずり
};
class Player :public GameObject
{
public:
	Player(){
		AddComponent("Rigidbody");
		isRigidbody = true;
	}
	~Player() { playerAnimations.clear(); }
	void Init(TextureManager& _textureManager);  //初期化
	void Update();//更新
	void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)override;  //描画
	void ResetJumpCnt()
	{
		jump_cnt = 0;
	}
private:
	Input& input = Input::GetInstance();
	std::unordered_map<AnimationState, std::shared_ptr<GameObject>> playerAnimations; // プレイヤーが管理するゲームオブジェクトリスト
	TextureManager* textureManager=nullptr;
	BoxCollider boxColl;
	const float scrollVelocity = 1.3f;//スクロールの速度
	int jump_cnt=0;
	bool enterRelease = false;
	bool landing = false;    //地面にいるか
	bool new_landing = false;//地面にいるか
	bool old_landing = false;//地面にいるか
	int playerHP = 3;//hp
	bool damage_flg = false;//ダメージを受けた時true
	const int invincibility_time =120;//無敵時間
	int invincibility_cnt = 0;
	int runUV_cnt = 0;

	int enterCnt = 0;
	bool drag_n = 0;
	bool drag_o = 0;
};