#include "Player.h"
#include "EventManager.h"
#include "sound.h"
#include "StageCollider.h"
#include "AttackAnimation.h"
#include "BigAttackAnimation1.h"
#include "BigAttackAnimation2.h"
#include "DragAnimation.h"
#include "BoxCollider.h"
#include "GameManager.h"

void Player::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 3, 2); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�

	//�R���|�[�l���g�̏�����
	for (auto& component : components)
	{
		component.second->Init(*this);
	}
	textureManager = &_textureManager;


	EventManager::GetInstance().AddListener("damage", [this]() {if (!damage_flg)this->playerHP--; damage_flg = true; });
	boxColl.Init(*this);
	//�v���C���[��HP��GameManager�ɓo�^
	GameManager::GetInstance().SetPlayerHP(playerHP);
}

void Player::Update(void)
{
	static int flame_cnt = 0;
	DirectX::XMFLOAT3 pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();

	//�v���C���[�̍��W��GameManager�ɓo�^
	GameManager::GetInstance().SetPlayerPos(pos);
	//�v���C���[��HP��GameManager�ɓo�^
	GameManager::GetInstance().SetPlayerHP(playerHP);

	//�L�����N�^�[�A�j���[�V����
	if (runUV_cnt == 4 && numU == 2)
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
	if (runUV_cnt == 4)
	{
		numU++;
		runUV_cnt = 0;
	}
	runUV_cnt++;

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
		_velocity.y = 3.0f;
		SetVelocity(_velocity);
		jump_cnt++;
		Sound::GetInstance().Play(SE_JUMP);
	}
	//�h���b�v
	if ((input.GetButtonTrigger(XINPUT_DOWN) && jump_cnt < 2) || (input.GetKeyTrigger(VK_S)))
	{
		StageCollider::GetInstance()->SetGhost();
	}


	if (landing==true)
	{
		EventManager::GetInstance().SendEvent("Vibration");

		//�������蒆
		if ((input.GetRightTriggerPress()) || (input.GetKeyPress(VK_SPACE)))
		{
			//��������g���K�[
			drag_o = drag_n;
			drag_n = true;
			if (drag_o == false && drag_n == true)
			{
				Sound::GetInstance().Play(SE_DRAG_ROOP);
				//Sound::GetInstance().Play(SE_SPARK_ROOP);
				playerAnimations[DRAG_ANI] = std::make_shared<DragAnimation>(*textureManager, pos);
			}

			enterCnt++;
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
					playerAnimations[DRAG_ANI]->SetTexture("asset/hikizuri2.png", TextureManager::GetInstance());
				}
				if (enterCnt > 120)
				{
					playerAnimations[DRAG_ANI]->SetPos(pos.x - 42, pos.y + 4, 0);
				}
			}
		}
		else{
			//��������̃t���O�̃��Z�b�g
			drag_o = false;
			drag_n = false;
		}

	}
	else
	{
		if (enterCnt != 0)
		{
			enterCnt--;
		}
	}
	if(enterCnt!=0)
	{
		//����������I�����Ă��ߍU��
		if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
		{
			Sound::GetInstance().Stop(SE_DRAG_ROOP);
			//Sound::GetInstance().Stop(SE_SPARK_ROOP);
			Sound::GetInstance().Play(SE_ATTACK_NORMAL);
			playerAnimations[ATTACK_ANI] = std::make_shared<AttackAnimation1>(*textureManager, pos);
			//�U���C�x���g�̑��M
			if (enterCnt < 60)
			{
				EventManager::GetInstance().SendEvent("normalAttack");
			}
			if (enterCnt >= 60 && enterCnt < 120)
			{
				playerAnimations[BIG_ATTACK_ANI1] = std::make_shared<BigAttackAnimation1>(*textureManager, pos);
				EventManager::GetInstance().SendEvent("attack1");
				Sound::GetInstance().Play(SE_ATTACK_FLAME);
			}
			if (enterCnt >= 120)
			{
				playerAnimations[BIG_ATTACK_ANI2] = std::make_shared<BigAttackAnimation2>(*textureManager, pos);
				EventManager::GetInstance().SendEvent("attack2");
				Sound::GetInstance().Play(SE_ATTACK_FLAME);
			}
			enterCnt = 0;

			enterRelease = true;
			playerAnimations.erase(DRAG_ANI);
		}
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
		if(land_check1 == true|| land_check2 == true)
		{
			landing = true;
		}
		else
		{
			landing = false;
			//��������̃��Z�b�g
			drag_o = false;
			drag_n = false;
			playerAnimations.erase(DRAG_ANI);
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
}

//��ΐ݌v
void Player::Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)
{
	for (const auto& pair : playerAnimations)
	{
		if (pair.second)pair.second->DrawObject(_vm, _pm);
	}
}