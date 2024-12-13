#include "Player.h"
#include "EventManager.h"
#include "sound.h"
#include "StageCollider.h"
#include "AttackAnimation.h"
#include "BigAttackAnimation1.h"
#include "BigAttackAnimation2.h"
#include "DragAnimation.h"

void Player::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager); //プレイヤーを初期化
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//角度を設定

	//コンポーネントの初期化
	for (auto& component : components)
	{
		component->Init(*this);
	}
	textureManager = &_textureManager;
}

void Player::Update(void)
{
	static int flame_cnt = 0;
	DirectX::XMFLOAT3 pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

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
	if (input.GetKeyTrigger(VK_W) && jump_cnt < 2)
	{
		_velocity.y = 3.0f;
		SetVelocity(_velocity);
		jump_cnt++;
	}
	//ドロップ
	if (input.GetKeyTrigger(VK_S))
	{
		StageCollider::GetInstance()->SetGhost();
	}

	static int enterCnt = 0;
	if (input.GetKeyTrigger(VK_SPACE))
	{
		Sound::GetInstance().Play(SOUND_LABEL_SE000); //BGMを再生定
		playerAnimations[DRAG_ANI] = std::make_shared<DragAnimation>(*textureManager, pos);
	}

	//引きずり中
	if (input.GetKeyPress(VK_SPACE))
	{
		enterCnt++;
		if(playerAnimations[DRAG_ANI] != nullptr)
		{
			if (enterCnt > 0 && enterCnt < 60)
			{
				playerAnimations[DRAG_ANI]->SetPos(pos.x - 25, pos.y - 9, 0);
				playerAnimations[DRAG_ANI]->SetColor(1.5f, 1.5f, 0.5f, 1.0f);
				playerAnimations[DRAG_ANI]->SetSize(20.0f, 20.0f, 0.0f);
			}
			if (enterCnt >= 60 && enterCnt < 120)
			{
				playerAnimations[DRAG_ANI]->SetPos(pos.x - 30, pos.y - 9, 0);
				playerAnimations[DRAG_ANI]->SetColor(2.0f, 1.0f, 1.0f, 1.0f);
				playerAnimations[DRAG_ANI]->SetSize(30.0f, 30.0f, 0.0f);
			}
			if (enterCnt >= 120)
			{
				playerAnimations[DRAG_ANI]->SetPos(pos.x - 35, pos.y - 9, 0);
				playerAnimations[DRAG_ANI]->SetColor(1.0f, 1.0f, 2.0f, 1.0f);
				playerAnimations[DRAG_ANI]->SetSize(40.0f, 40.0f, 0.0f);
			}
		}
	}

	//引きずりを終了してため攻撃
	if (input.GetKeyRelease(VK_SPACE))
	{
		//攻撃イベントの送信
 		playerAnimations[ATTACK_ANI] = std::make_shared<AttackAnimation1>(*textureManager, pos);
		if (enterCnt < 60)
		{
			EventManager::GetInstance().SendEvent("normalAttack");
		}
		if (enterCnt >= 60 && enterCnt < 120)
		{
			playerAnimations[BIG_ATTACK_ANI1] = std::make_shared<BigAttackAnimation1>(*textureManager, pos);
			EventManager::GetInstance().SendEvent("attack1");
		}
		if (enterCnt >= 120)
		{
			playerAnimations[BIG_ATTACK_ANI2] = std::make_shared<BigAttackAnimation2>(*textureManager, pos);
			EventManager::GetInstance().SendEvent("attack2");
		}
		enterCnt = 0;
		Sound::GetInstance().Stop(SOUND_LABEL_SE000); //BGMを再生定
		Sound::GetInstance().Play(SOUND_LABEL_SE001); //BGMを再生定

		enterRelease = true;
		playerAnimations.erase(DRAG_ANI);
	}
	//プレイヤーの速度の更新
	_velocity.x = scrollVelocity;
	SetVelocity(_velocity);
	//プレイヤーの座標計算
	SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);

	//コンポーネントの更新
	for (auto& component : components)
	{
		component->Update();
	}
	//ステージオブジェクトとの当たり判定
	StageCollider::GetInstance()->HitCheck(*this);

	//アニメーションを消す
	if (playerAnimations.size() != 0&& enterRelease==true)
	{
		flame_cnt++;
		if (flame_cnt == 30)
		{
			playerAnimations.erase(ATTACK_ANI);
			playerAnimations.erase(BIG_ATTACK_ANI1);
			playerAnimations.erase(BIG_ATTACK_ANI2);
			flame_cnt = 0;
			enterRelease = false;
		}
	}

	//アニメーションのアップデート
	for (const auto& pair : playerAnimations)
	{
		if(pair.second)pair.second->Update();
	}

}

//やば設計
void Player::Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	for (const auto& pair : playerAnimations)
	{
		if (pair.second)pair.second->DrawObject(_vm, _pm);
	}
}