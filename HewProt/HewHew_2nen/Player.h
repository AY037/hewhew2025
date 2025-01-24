#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
enum PlayerAnimationState
{
	RUN,
	ATTACK,
	JUMP,
	SLIDE
};

enum AnimationState
{
	ATTACK_ANI=0,		//通常攻撃
	BIG_ATTACK_ANI0,//通常衝撃波
	BIG_ATTACK_ANI1,//ため攻撃
	BIG_ATTACK_ANI2,//強ため攻撃
	DRAG_ANI		//引きずり
};

enum LandingState
{
	SLIDING,//坂を滑り中
	LANDING,//床を井戸中
	NORMAL, //通常状態
};

struct Land {
	LandingState landState=NORMAL;
	bool changeTexture = false;
	float posX =0.0f;//角度が０に戻るまでの距離
};

class Player :public GameObject
{
public:
	Player();
	~Player();
	void Init();  //初期化
	void Update();//更新
	void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)override;  //描画
	void ResetJumpCnt()
	{
		jump_cnt = 0;
	}
	DirectX::XMFLOAT3 GetBoxSize()override;

	void ResetTexture();//テクスチャを通常状態へ
private:
	Input& input = Input::GetInstance();
	std::unordered_map<AnimationState, std::shared_ptr<GameObject>> playerAnimations; // プレイヤーが管理するゲームオブジェクトリスト
	BoxCollider boxColl;
	float scrollVelocity = 1.3f;//スクロールの速度
	int jump_cnt=0;
	bool enterRelease = false;
	bool landing = false;    //地面にいるか
	bool new_landing = false;//地面にいるか
	bool old_landing = false;//地面にいるか
	int playerHP = 300;//hp
	bool damage_flg = false;//ダメージを受けた時true
	const int invincibility_time =120;//無敵時間
	int invincibility_cnt = 0;
	int runUV_cnt = 0;

	int enterCnt = 0;
	int enterCnt_Old = 0;
	bool drag_n = 0;
	bool drag_o = 0;

	int m_Flame_cnt = 0;

	bool m_AttackFlg = false;
	int m_AttackCnt = 0;

	int m_UpdateAniFlame = 4;

	bool drawDragAni = false;//引きずりの描画
	int slopeId = -1;//滑ってる坂のid
	Land land;
	PlayerAnimationState playerAnimationState;
};