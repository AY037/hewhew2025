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
#include "Math.h"

using namespace DirectX::SimpleMath;

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
	Initialize(GetObjectTexName(), 3, 2); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�

	//�R���|�[�l���g�̏�����
	for (auto& component : components)
	{
		component.second->Init(*this);
	}

	EventManager::GetInstance().AddListener("damage", [this]() {
		if (!damage_flg && playerAnimationState != SLIDE)
		{
			this->playerHP -= 50.0f;
			damage_flg = true;
			AudioManager::GetInstance().PlayAudio(SE_VOICE_DAMAGE_NORMAL1, false);
		}
		});

	//��ʒ[�Ƌ��܂������̓��ߏ���
	EventManager::GetInstance().AddListener("Transparent", [this]() {
		DynamicAABBTree::GetInstance().updateAABB(GetObjID());
		std::vector<std::shared_ptr<GameObject>> hitObjectNames = boxColl.HitObjectName(*this);

		int groundId =-1;
		float height = 10000.0f;//�ڐG���Ă�I�u�W�F�N�g�̒��ōł��Ⴂ�I�u�W�F�N�g��y���W

		//�����蔻���n�ʂ����ɂ���
		for (auto& obj : hitObjectNames)
		{
			std::string objTypeName = obj->GetObjTypeName();

			if (objTypeName == "Ground"|| objTypeName == "Stage")
			{
				if (height > obj->GetPos().y)
				{
					groundId = obj->GetObjID();
					height = obj->GetPos().y;
				}
			}

			if (groundId != -1)
			{
				slopeId = groundId;
				land.landState = TRANSPARENCY;
			}
		}
		});

	playerAnimations[DRAG_ANI] = std::make_shared<DragAnimation>(GetPos());
	boxColl.Init(*this);
	//�v���C���[��HP��GameManager�ɓo�^
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
	//�v���C���[�̍��W��GameManager�ɓo�^
	gameManager.SetPlayerPos(pos);
	//�v���C���[��HP��GameManager�ɓo�^
	gameManager.SetPlayerHP(playerHP);
	gameManager.dragCharging = enterCnt;

	//�v���C���[�̃A�j���[�V����
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

	//�h���b�v
	if ((input.GetButtonTrigger(XINPUT_DOWN) || (input.GetKeyTrigger(VK_S))) && jump_cnt == 0 && !m_AttackFlg)
	{
		StageCollider::GetInstance()->SetGhost();

		_velocity.y = -1.0f;
	}


	//�v���C���[�̑��x�̍X�V
	_velocity.x = scrollVelocity;
	SetVelocity(_velocity);
	//�v���C���[�̍��W�v�Z
	SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);

	//�������蒆
	if (gameManager.dragSwordHit)
	{
		EventManager::GetInstance().SendEvent("Vibration");

		AudioManager::GetInstance().PlayAudio(SE_DRAG_ROOP, true);
		drawDragAni = true;

		//��������g���K�[
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
				//�����x�N�g���̉�]
				float radAngle = DirectX::XMConvertToRadians(GetAngle());
				playerAnimations[DRAG_ANI]->SetAngle(GetAngle());
				if (enterCnt == 0)
				{
					playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri.png");
				}
				if (enterCnt > 0 && enterCnt < 60)
				{
					Vector3 adjustVec = math::GetRotateVec(-32.0f, -6.0f, 0.0f, radAngle, math::Z);
					playerAnimations[DRAG_ANI]->SetPos(pos.x + adjustVec.x, pos.y + adjustVec.y, 0.0f);
					playerAnimations[DRAG_ANI]->SetSize(30.0f, 30.0f, 0.0f);
				}
				if (enterCnt >= 60 && enterCnt < 120)
				{
					Vector3 adjustVec = math::GetRotateVec(-38.0f, 2.0f, 0.0f, radAngle, math::Z);
					playerAnimations[DRAG_ANI]->SetPos(pos.x + adjustVec.x, pos.y + adjustVec.y, 0.0f);
					playerAnimations[DRAG_ANI]->SetSize(40.0f, 40.0f, 0.0f);
				}
				if (enterCnt == 120)
				{
					playerAnimations[DRAG_ANI]->SetSize(50.0f, 50.0f, 0.0f);
					playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri2.png");
				}
				if (enterCnt > 120)
				{
					Vector3 adjustVec = math::GetRotateVec(-42.0f, 4.0f, 0.0f, radAngle, math::Z);
					playerAnimations[DRAG_ANI]->SetSize(50.0f, 50.0f, 0.0f);
					playerAnimations[DRAG_ANI]->SetPos(pos.x + adjustVec.x, pos.y + adjustVec.y, 0.0f);
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
		//��������̃��Z�b�g
		drag_o = false;
		drag_n = false;
	}


	if (enterCnt != 0)
	{
		//����������I�����Ă��ߍU��
		if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
		{
			if (playerAnimationState == SLIDE)
			{
				ResetTexture();
			}

			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
			//��������̃t���O�̃��Z�b�g
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
		EventManager::GetInstance().SendEvent("normalAttack");
		//�U���C�x���g�̑��M
		if (enterCnt < 60)
		{
			DirectX::XMFLOAT3 swordAniPos = DirectX::XMFLOAT3(pos.x + size.x / 2, pos.y, pos.z);
			playerAnimations[BIG_ATTACK_ANI0] = std::make_shared<BigAttackAnimation0>(swordAniPos);
			EventManager::GetInstance().SendEvent("attack0");
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

	//��ʊO�Ƀv���C���[���o����
	if (pos.x < gameManager.cameraPos.x - 160)
	{
		EventManager::GetInstance().SendEvent("Transparent");//������
		EventManager::GetInstance().SendEvent("damage");
	}

	DynamicAABBTree::GetInstance().updateAABB(GetObjID());
	std::vector<std::shared_ptr<GameObject>> hitObjectNames = boxColl.HitObjectName(*this);

	for (auto& obj : hitObjectNames)
	{
		std::string objTypeName = obj->GetObjTypeName();
		auto hitObjPos = obj->GetPos();
		if (objTypeName == "Ground")
		{
			float angle = obj->GetAngle();
			if (angle != 0)
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

		//��������Ȃ������ړ�����ΓG���c�[��
		if (objTypeName == "Enemy" && playerAnimationState == SLIDE)
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
				if (!m_AttackFlg&&GetSize().y!=30.0f)
				{
					//�v���C���[�̍��W�v�Z
					SetTexture("asset/player_Slide.png");
					playerAnimationState = SLIDE;
					land.landState = SLIDING;
					SetUV(0, 0, 1, 1);
					SetSize(30.0f, 30.0f, 0.0f);
					SetPos(pos.x + 10.0f, pos.y + 10.0f, 0.0f);
					land.changeTexture = true;
				}
				//slopeId = -1;//�����͕����I�u�W�F�N�g�Ɠ����蔻�����邽��
			}
			else
			{
				land.landState = NORMAL;
				if (!land.changeTexture && !m_AttackFlg)
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
			//�v���C���[�̍��W�v�Z
			SetPos(pos.x + _velocity.x, pos.y + _velocity.y, pos.z + _velocity.z);
		}
	}

	//�≺��������莞�̏���
	if (playerAnimationState == SLIDE && drawDragAni)
	{
		AudioManager::GetInstance().PlayAudio(SE_DRAG_ROOP, true);
		float radAngle = DirectX::XMConvertToRadians(GetAngle());
		Vector3 adjustVec = math::GetRotateVec(-30.0f, 0.0f, 0.0f, radAngle, math::Z);
		playerAnimations[DRAG_ANI]->SetPos(pos.x + adjustVec.x, pos.y + adjustVec.y, 0.0f);
		playerAnimations[DRAG_ANI]->SetAngle(GetAngle());
		playerAnimations[DRAG_ANI]->SetSize(40.0f, 40.0f, 0.0f);
	}

	//��̉E�[�ɓ��B������ʏ��Ԃɖ߂�
	auto vertexPos = GetVertexPos();
	if (vertexPos.pos[RIGHT_TOP].x > land.posX)
	{
		if (GetAngle() != 0.0f)
		{
			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
			ResetTexture();
		}
	}

	//=======================================================================
	//�R���|�[�l���g�̍X�V
	//=======================================================================


	bool land_check1 = false;
	//�X�e�[�W�I�u�W�F�N�g�Ƃ̓����蔻��
	if (land.landState == NORMAL)
	{
		land_check1 = boxColl.HitCheck(*this);
	}
	else
	{
		if (slopeId != -1)
		{
			//��I�u�W�F�N�g�̂ݓ����蔻��
			land_check1 = boxColl.HitCheck(*this, slopeId);
		}
		else
		{
			land_check1 = boxColl.HitCheck(*this);
		}
	}
	//���̑��Ƃ̓����蔻��
	bool land_check2 = StageCollider::GetInstance()->HitCheck(*this);
	//�����Rigidbody�̂�
	for (auto& component : components)
	{
		if (!m_AttackFlg)
		{
			component.second->Update();
		}
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
			AudioManager::GetInstance().StopAudio(SE_DRAG_ROOP);
		}
	}


	//�A�j���[�V����������
	if (playerAnimations.size() != 0 && enterRelease == true)
	{
		flame_cnt++;
		if (flame_cnt == 32)
		{
			EventManager::GetInstance().SendEvent("deleteSword");
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

		//�I�u�W�F�N�g���ߏ�Ԃ̎��̂�NORMAL�ɖ߂�
		if(land.landState==TRANSPARENCY)
		{
			land.landState = NORMAL;
		}
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
