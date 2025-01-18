#include "Player.h"
#include "sound.h"
#include "EventManager.h"
#include "StageCollider.h"
#include "AttackAnimation.h"
#include "BigAttackAnimation1.h"
#include "BigAttackAnimation2.h"
#include "DragAnimation.h"
#include "BoxCollider.h"
#include "GameManager.h"
#include "BigAttackAnimation0.h"

Player::Player()
{
	AddComponent("Rigidbody");
	SetObjTypeName("Player");
	SetTexture("asset/player_Attack.png", TextureManager::GetInstance());
	isRigidbody = true;
}

Player::~Player() {
	playerAnimations.clear();
	Sound::GetInstance().Stop(SE_DRAG_ROOP);

}

void Player::Init()
{
	Initialize(GetObjectTexName(),3,2); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�

	//�R���|�[�l���g�̏�����
	for (auto& component : components)
	{
		component.second->Init(*this);
	}

	EventManager::GetInstance().AddListener("damage", [this]() {if (!damage_flg)this->playerHP -= 50.0f; damage_flg = true; });
	boxColl.Init(*this);
	//�v���C���[��HP��GameManager�ɓo�^
	GameManager::GetInstance().SetPlayerHP(playerHP);
}

void Player::Update(void)
{
	static int flame_cnt = 0;
	DirectX::XMFLOAT3 pos = GetPos();
	DirectX::XMFLOAT3 size = GetSize();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	auto& gameManager = GameManager::GetInstance();
	//�v���C���[�̍��W��GameManager�ɓo�^
	gameManager.SetPlayerPos(pos);
	//�v���C���[��HP��GameManager�ɓo�^
	gameManager.SetPlayerHP(playerHP);
	gameManager.dragCharging = enterCnt;

	//�v���C���[�̃A�j���[�V����

	if(!m_AttackFlg)
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
	}
	else
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
	}
	//�f�o�b�O�p----------------------------------------
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

	//�W�����v
	if ((input.GetButtonTrigger(XINPUT_UP) || (input.GetKeyTrigger(VK_W))) && jump_cnt < 2)
	{
		_velocity.y = 4.5f;
		SetVelocity(_velocity);
		jump_cnt++;
		Sound::GetInstance().Play(SE_JUMP);
	}
	//�h���b�v
	if ((input.GetButtonTrigger(XINPUT_DOWN) || (input.GetKeyTrigger(VK_S))) && jump_cnt == 0)
	{
		StageCollider::GetInstance()->SetGhost();

		_velocity.y = -1.0f;
	}


	if (gameManager.dragSwordHit)
	{
		EventManager::GetInstance().SendEvent("Vibration");

		//�������蒆
		if ((input.GetRightTriggerPress()) || (input.GetKeyPress(VK_SPACE)) && !m_AttackFlg)
		{
			//��������g���K�[
			drag_o = drag_n;
			drag_n = true;
			if (drag_o == false && drag_n == true)
			{
				Sound::GetInstance().Play(SE_DRAG_ROOP);
				//Sound::GetInstance().Play(SE_SPARK_ROOP);
				playerAnimations[DRAG_ANI] = std::make_shared<DragAnimation>(pos);
			}
			if (enterCnt != 180)
			{
				enterCnt++;
			}
			if (playerAnimations[DRAG_ANI] != nullptr)
			{
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
					if (enterCnt_Old < 120)
					{
						playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri2.png", TextureManager::GetInstance());
					}
					if (enterCnt_Old > 120)
					{
						//playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri.png", TextureManager::GetInstance());
					}
				}
				if (enterCnt > 120)
				{
					playerAnimations[DRAG_ANI]->SetSize(50.0f, 50.0f, 0.0f);
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 42, pos.y + 4, 0);
				}
			}
		}
		else
		{
			//��������̃t���O�̃��Z�b�g
			drag_o = false;
			drag_n = false;
		}

	}
	else
	{
		if (enterCnt != 0 && m_Flame_cnt % 2 == 0)
		{
			enterCnt--;
		}
		playerAnimations.erase(DRAG_ANI);
		//��������̃��Z�b�g
		drag_o = false;
		drag_n = false;
	}
	if (enterCnt != 0)
	{
		//����������I�����Ă��ߍU��
		if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
		{
			Sound::GetInstance().Stop(SE_DRAG_ROOP);
			enterRelease = true;
			m_AttackFlg = true;
			playerAnimations.erase(DRAG_ANI);
			SetTexture("asset/player_Attack.png", TextureManager::GetInstance());
			numU = 0;
			numV = 0;
			runUV_cnt = 0;
			m_UpdateAniFlame = 6;
			SetSize(size.x * 2, size.y * 2, 0.0f);
		}
	}

	if (m_AttackCnt==12)
	{
		//Sound::GetInstance().Stop(SE_SPARK_ROOP);
		Sound::GetInstance().Play(SE_ATTACK_NORMAL);
		playerAnimations[ATTACK_ANI] = std::make_shared<AttackAnimation1>(pos);
		//�U���C�x���g�̑��M
		if (enterCnt < 60)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI0] = std::make_shared<BigAttackAnimation0>(swordAniPos);
			EventManager::GetInstance().SendEvent("normalAttack");
		}
		if (enterCnt >= 60 && enterCnt < 120)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI1] = std::make_shared<BigAttackAnimation1>(swordAniPos);
			EventManager::GetInstance().SendEvent("attack1");
			Sound::GetInstance().Play(SE_ATTACK_FLAME);
		}
		if (enterCnt >= 120)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI2] = std::make_shared<BigAttackAnimation2>(swordAniPos);
			EventManager::GetInstance().SendEvent("attack2");
			Sound::GetInstance().Play(SE_ATTACK_FLAME);
		}
		enterCnt = 0;
	}


	if (m_AttackFlg)
	{
		if (m_AttackCnt == 42)
		{
			m_AttackFlg = false;
			m_AttackCnt = 0;
			SetTexture("asset/player_Run.png", TextureManager::GetInstance());
			SetSize(size.x / 2, size.y / 2, 0.0f);
			numU = 0;
			numV = 0;
			runUV_cnt = 0;
			m_UpdateAniFlame = 4;
		}
		m_AttackCnt++;
	}

	//�v���C���[�̑��x�̍X�V
	_velocity.x = scrollVelocity;
	SetVelocity(_velocity);
	//�v���C���[�̍��W�v�Z
	SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);

	//=======================================================================
	//�R���|�[�l���g�̍X�V
	//=======================================================================

	//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
	bool land_check1 = boxColl.HitCheck(*this);
	std::unordered_map<std::string, std::shared_ptr<GameObject>> hitObjectNames = boxColl.HitObjectName(*this);
	for (auto& pair : hitObjectNames)
	{
		if (pair.first == "Ground")
		{
			auto& obj = pair.second;

			float angle = obj->GetAngle();
			if (changeAngleCnt == 0 && angle > 180 && angle < 360)
			{
				//SetAngle(angle);
				changeAngle = true;
			}

			if (obj->GetName() == "DontDragStage")
			{
				//scrollVelocity = 0.65;
				enterCnt = 0;
			}
			else
			{
				//scrollVelocity = 1.3f;
			}
		}
	}

	if (changeAngle)
	{
		changeAngleCnt++;
		if (changeAngleCnt == 60)
		{
			changeAngleCnt = 0;
			changeAngle = false;
		}
	}

	//���̑��Ƃ̓����蔻��
	bool land_check2 = StageCollider::GetInstance()->HitCheck(*this);
	//�����Rigidbody�̂�
	for (auto& component : components)
	{
		component.second->Update();
	}
	//�n�ʂƂ̐ڐG����
	if (runUV_cnt == 4)
	{
		if (land_check1 == true || land_check2 == true)
		{
			landing = true;
		}
		else
		{
			landing = false;
			Sound::GetInstance().Stop(SE_DRAG_ROOP);
		}
	}


	//�A�j���[�V����������
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

	//�A�j���[�V�����̃A�b�v�f�[�g
	for (const auto& pair : playerAnimations)
	{
		if (pair.second)pair.second->Update();
	}

	//�_���[�W�󂯂����̓_��
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

//��ΐ݌v
void Player::Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	for (const auto& pair : playerAnimations)
	{
		if (pair.second)pair.second->DrawObject(_vm, _pm);
	}
}

DirectX::XMFLOAT3 Player::GetBoxSize()
{
	if(m_AttackFlg)
	{
		return DirectX::XMFLOAT3(46.6, 40, 0);
	}
	else
	{
		return DirectX::XMFLOAT3(46.6, 20, 0);
	}
}