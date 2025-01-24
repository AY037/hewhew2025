#include "Player.h"
#include "EventManager.h"
#include "StageCollider.h"
#include "AttackAnimation.h"
#include "BigAttackAnimation1.h"
#include "BigAttackAnimation2.h"
#include "DragAnimation.h"
#include "BoxCollider.h"
#include "GameManager.h"
#include "BigAttackAnimation0.h"
#include "AudioManager.h"

Player::Player()
{
	AddComponent("Rigidbody");
	SetObjTypeName("Player");
	SetTexture("asset/player_Slide.png");
	SetTexture("asset/player_Jump.png");
	SetTexture("asset/player_Attack.png");
	isRigidbody = true;
}

Player::~Player() {
	playerAnimations.clear();
}

void Player::Init()
{
	Initialize(GetObjectTexName(), 3, 2); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定

	//コンポーネントの初期化
	for (auto& component : components)
	{
		component.second->Init(*this);
	}

	EventManager::GetInstance().AddListener("damage", [this]() {
		if (!damage_flg && land.landState != SLIDING)
		{
			this->playerHP -= 50.0f;
			damage_flg = true;
			AudioManager::GetInstance().PlayAudio(SE_VOICE_DAMAGE_NORMAL1, false);
		}
		});
	playerAnimations[DRAG_ANI] = std::make_shared<DragAnimation>(GetPos());
	boxColl.Init(*this);
	//プレイヤーのHPをGameManagerに登録
	GameManager::GetInstance().SetPlayerHP(playerHP);
	playerAnimationState = RUN;

	GameManager::GetInstance().SetPlayerPos(GetPos());
}

void Player::Update(void)
{
	static int flame_cnt = 0;
	DirectX::XMFLOAT3 pos = GetPos();
	DirectX::XMFLOAT3 size = GetSize();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	auto& gameManager = GameManager::GetInstance();
	//プレイヤーの座標をGameManagerに登録
	gameManager.SetPlayerPos(pos);
	//プレイヤーのHPをGameManagerに登録
	gameManager.SetPlayerHP(playerHP);
	gameManager.dragCharging = enterCnt;

	//プレイヤーのアニメーション

	switch (playerAnimationState)
	{
	case RUN:
	{
		if (runUV_cnt == m_UpdateAniFlame && numU == 2)
		{
			if (numV == 0)
			{
				numV = 1;
			}
			else
			{
				numV = 0;
			}
			numU = 0;
		}
		if (runUV_cnt == m_UpdateAniFlame)
		{
			numU++;
			runUV_cnt = 0;
		}
		runUV_cnt++;
		break;
	}
	case ATTACK:
	{
		if (runUV_cnt == m_UpdateAniFlame && numU == 2)
		{
			if (numV == 0)
			{
				numV = 1;
				m_UpdateAniFlame = 8;
			}
			else
			{
				numV = 0;
			}
			numU = 0;
		}
		if (runUV_cnt == m_UpdateAniFlame)
		{
			numU++;
			runUV_cnt = 0;
		}
		runUV_cnt++;
		break;
	}
	case JUMP:
	{
		m_UpdateAniFlame = 6;
		if (runUV_cnt == m_UpdateAniFlame && numU == 2)
		{
			if (numV == 0)
			{
				numV = 1;
			}
			else
			{
				numV = 1;
				numU = 4;
			}
			numU = 0;
		}
		if (runUV_cnt == m_UpdateAniFlame)
		{
			numU++;
			runUV_cnt = 0;
		}
		runUV_cnt++;
		break;
	}
	default:
	{
		break;
	}
	}
	//デバッグ用----------------------------------------
	//if (input.GetKeyPress(VK_D))
	//{
	//	_velocity.x = 2;
	//	SetVelocity(_velocity);
	//}
	//if (input.GetKeyPress(VK_A))
	//{
	//	_velocity.x = -2;
	//	SetVelocity(_velocity);
	//}
	//--------------------------------------------------

	//ジャンプ
	if ((input.GetButtonTrigger(XINPUT_UP) || (input.GetKeyTrigger(VK_W))) && jump_cnt < 2 && !m_AttackFlg)
	{
		if (playerAnimationState == SLIDE)
		{
			ResetTexture();
		}
		_velocity.y = 4.5f;
		SetVelocity(_velocity);
		jump_cnt++;
		AudioManager::GetInstance().PlayAudio(SE_JUMP, false);
		SetTexture("asset/player_Jump.png");
		numU = 0;
		numV = 0;
		runUV_cnt = 0;
		SetSize(GetSize().x, 30.0f, 0.0f);
		playerAnimationState = JUMP;
		land.landState = NORMAL;
	}

	//ドロップ
	if ((input.GetButtonTrigger(XINPUT_DOWN) || (input.GetKeyTrigger(VK_S))) && jump_cnt == 0 && !m_AttackFlg)
	{
		StageCollider::GetInstance()->SetGhost();

		_velocity.y = -1.0f;
	}


	//プレイヤーの速度の更新
	_velocity.x = scrollVelocity;
	SetVelocity(_velocity);
	//プレイヤーの座標計算
	SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);

	//引きずり中
	if (gameManager.dragSwordHit)
	{
		EventManager::GetInstance().SendEvent("Vibration");

		AudioManager::GetInstance().PlayAudio(SE_DRAG_ROOP, true);
		drawDragAni = true;

		//引きずりトリガー
		drag_o = drag_n;
		drag_n = true;
		if (drag_o == false && drag_n == true)
		{
			//Sound::GetInstance().Play(SE_SPARK_ROOP);
		}
		if (!m_AttackFlg)
		{
			if (playerAnimations[DRAG_ANI] != nullptr)
			{
				if (enterCnt == 0)
				{
					playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri.png");
				}
				if (enterCnt > 0 && enterCnt < 60)
				{
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 32, pos.y - 6, 0);
					playerAnimations[DRAG_ANI]->SetSize(30.0f, 30.0f, 0.0f);
				}
				if (enterCnt >= 60 && enterCnt < 120)
				{
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 38, pos.y + 2, 0);
					playerAnimations[DRAG_ANI]->SetSize(40.0f, 40.0f, 0.0f);
				}
				if (enterCnt == 120)
				{
					playerAnimations[DRAG_ANI]->SetSize(50.0f, 50.0f, 0.0f);
					playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri2.png");
				}
				if (enterCnt > 120)
				{
					playerAnimations[DRAG_ANI]->SetSize(50.0f, 50.0f, 0.0f);
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 42, pos.y + 4, 0);
				}
			}
			if (enterCnt != 180)
			{
				enterCnt++;
			}
		}
	}
	else
	{
		if (enterCnt != 0 && m_Flame_cnt % 2 == 0)
		{
			enterCnt--;
		}
		drawDragAni = false;
		//引きずりのリセット
		drag_o = false;
		drag_n = false;
	}


	if (enterCnt != 0)
	{
		//引きずりを終了してため攻撃
		if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
		{
			if (playerAnimationState == SLIDE)
			{
				ResetTexture();
			}

			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
			//引きずりのフラグのリセット
			drag_o = false;
			drag_n = false;
			enterRelease = true;
			m_AttackFlg = true;
			playerAnimationState = ATTACK;
			SetTexture("asset/player_Attack.png");
			numU = 0;
			numV = 0;
			runUV_cnt = 0;
			m_UpdateAniFlame = 6;
			SetSize(93.2f, 40.0f, 0.0f);
			land.changeTexture = false;
		}
	}

	if (m_AttackCnt == 12)
	{
		//Sound::GetInstance().Stop(SE_SPARK_ROOP);
		AudioManager::GetInstance().PlayAudio(SE_ATTACK_NORMAL, false);
		playerAnimations[ATTACK_ANI] = std::make_shared<AttackAnimation1>(pos);
		//攻撃イベントの送信
		if (enterCnt < 60)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI0] = std::make_shared<BigAttackAnimation0>(swordAniPos);
			EventManager::GetInstance().SendEvent("normalAttack");
			AudioManager::GetInstance().PlayAudio(SE_VOICE_ATTACK_NORMAL, false);
		}
		if (enterCnt >= 60 && enterCnt < 120)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI1] = std::make_shared<BigAttackAnimation1>(swordAniPos);
			EventManager::GetInstance().SendEvent("attack1");
			AudioManager::GetInstance().PlayAudio(SE_ATTACK_FLAME, false);
			AudioManager::GetInstance().PlayAudio(SE_VOICE_ATTACK_BIGATTACK1, false);
		}
		if (enterCnt >= 120)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI2] = std::make_shared<BigAttackAnimation2>(swordAniPos);
			EventManager::GetInstance().SendEvent("attack2");
			AudioManager::GetInstance().PlayAudio(SE_ATTACK_FLAME, false);
			AudioManager::GetInstance().PlayAudio(SE_VOICE_ATTACK_BIGATTACK2, false);
		}
		enterCnt = 0;
	}


	if (m_AttackFlg)
	{
		if (m_AttackCnt == 1)
		{
			SetPos(pos.x, pos.y + 10, 0.0f);
			SetVelocity(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		}
		if (m_AttackCnt == 42)
		{
			m_AttackFlg = false;
			m_AttackCnt = 0;
			ResetTexture();
		}
		m_AttackCnt++;
	}



	DynamicAABBTree::GetInstance().updateAABB(GetObjID());
	std::vector<std::shared_ptr<GameObject>> hitObjectNames = boxColl.HitObjectName(*this);

	for (auto& obj : hitObjectNames)
	{
		std::string objTypeName = obj->GetObjTypeName();

		if (objTypeName == "Ground")
		{
			bool checkNormalGround = false;
			for (auto& checkObj : hitObjectNames)
			{
				if (checkObj->GetObjTypeName() == "Ground" || checkObj->GetObjTypeName() == "Stage")
				{
					if (checkObj->GetAngle() == 0)
					{
						checkNormalGround = true;
					}
				}
			}

			float angle = obj->GetAngle();
			if (angle != 0 && objTypeName != "FallWall")
			{
				if (land.landState == NORMAL)
				{
					auto vertexPos = obj->GetVertexPos();
					land.posX = vertexPos.pos[RIGHT_TOP].x;
					if (GetVertexPos().pos[RIGHT_TOP].x < land.posX)
					{
						SetAngle(angle);
						land.landState = SLIDING;
						slopeId = obj->GetObjID();
					}
				}
			}

			if (objTypeName == "DontDragStage")
			{
				//scrollVelocity = 0.65;
				enterCnt = 0;
			}
			else
			{
				//scrollVelocity = 1.3f;
			}
		}

		if (objTypeName == "Stage" || objTypeName == "Ground")
		{
			if (jump_cnt == 0 && playerAnimationState == JUMP)
			{
				ResetTexture();
			}
		}

		if (objTypeName == "Enemy" && land.landState == SLIDING)
		{
			EventManager::GetInstance().SendObjIdEvent("TransDebri", obj->GetObjID());
		}
	}

	if (land.landState == SLIDING)
	{
		auto pos = GetPos();
		float angle = GetAngle();
		if (angle > 180 && angle < 360)
		{
			if (gameManager.dragSwordHit)
			{
				if (!m_AttackFlg)
				{
					//プレイヤーの座標計算
					SetTexture("asset/player_Slide.png");
					playerAnimationState = SLIDE;
					land.landState = SLIDING;
					SetUV(0, 0, 1, 1);
					SetSize(30.0f, 30.0f, 0.0f);
					land.changeTexture = true;
				}
				AudioManager::GetInstance().PlayAudio(SE_DRAG_ROOP, true);
				if (gameManager.dragSwordHit)
				{
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 20, pos.y - 5, 0);
				}
				slopeId = -1;//下り坂は複数オブジェクトと当たり判定を取るため
			}
			else
			{
				land.landState = NORMAL;
				if(!land.changeTexture&&!m_AttackFlg)
				{
					ResetTexture();
					land.changeTexture = true;
				}
			}
		}
		else
		{
			_velocity.x = 0.2;
			_velocity.y = 0.1f;
			SetVelocity(_velocity);
			//プレイヤーの座標計算
			SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);
		}
	}


	//坂の右端に到達したら通常状態に戻す
	auto vertexPos = GetVertexPos();
	if (vertexPos.pos[RIGHT_TOP].x > land.posX)
	{
		if (GetAngle()!=0.0f)
		{
			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
			ResetTexture();
		}
	}

	//=======================================================================
	//コンポーネントの更新
	//=======================================================================


	bool land_check1 = false;
	//ステージオブジェクトとの当たり判定
	if (land.landState == NORMAL)
	{
		land_check1 = boxColl.HitCheck(*this);
	}
	else
	{
		if (slopeId != -1)
		{
			//坂オブジェクトのみ当たり判定
			land_check1 = boxColl.HitCheck(*this, slopeId);
		}
		else
		{
			land_check1 = boxColl.HitCheck(*this);
		}
	}
	//その他との当たり判定
	bool land_check2 = StageCollider::GetInstance()->HitCheck(*this);
	//現状はRigidbodyのみ
	for (auto& component : components)
	{
		if (!m_AttackFlg)
		{
			component.second->Update();
		}
	}
	//地面との接触判定
	if (runUV_cnt == 4)
	{
		if (land_check1 == true || land_check2 == true)
		{
			landing = true;
		}
		else
		{
			landing = false;
			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
		}
	}


	//アニメーションを消す
	if (playerAnimations.size() != 0 && enterRelease == true)
	{
		flame_cnt++;
		if (flame_cnt == 25)
		{
			playerAnimations.erase(ATTACK_ANI);
			playerAnimations.erase(BIG_ATTACK_ANI0);
			playerAnimations.erase(BIG_ATTACK_ANI1);
			playerAnimations.erase(BIG_ATTACK_ANI2);
			flame_cnt = 0;
			enterRelease = false;
		}
	}

	//アニメーションのアップデート
	for (const auto& pair : playerAnimations)
	{
		if (pair.second)pair.second->Update();
	}

	//ダメージ受けた時の点滅
	if (damage_flg == true)
	{
		invincibility_cnt++;
		if (invincibility_cnt % 8 == 0)
		{
			SetColor(1.0f, 1.0f, 1.0f, 0.5f);
		}
		if (invincibility_cnt % 16 == 0)
		{
			SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	if (invincibility_cnt == invincibility_time)
	{
		damage_flg = false;
		invincibility_cnt = 0;
		SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (playerHP == 0)
	{
		EventManager::GetInstance().SendChangeScene("ResultScene");
	}

	if (m_Flame_cnt == 60)
	{
		m_Flame_cnt = 0;
	}
	//
	m_Flame_cnt++;
	enterCnt_Old = enterCnt;
}

//やば設計
void Player::Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	for (const auto& pair : playerAnimations)
	{
		if (pair.first == DRAG_ANI && !drawDragAni)continue;
		if (pair.second)pair.second->DrawObject(_vm, _pm);
	}
}

DirectX::XMFLOAT3 Player::GetBoxSize()
{
	//if(m_AttackFlg)
	//{
		//return DirectX::XMFLOAT3(46.6, 40, 0);
	//}
	//else
	//{
	return DirectX::XMFLOAT3(46.6, 20, 0);
	//}
}

void Player::ResetTexture()
{
	SetAngle(0);
	land.landState = NORMAL;
	playerAnimationState = RUN;
	SetSize(46.6f, 20.0f, 0.0f);
	SetUV(0, 0, 3, 2);
	SetTexture("asset/player_Run.png");
	numU = 0;
	numV = 0;
	runUV_cnt = 0;
	m_UpdateAniFlame = 4;
}
