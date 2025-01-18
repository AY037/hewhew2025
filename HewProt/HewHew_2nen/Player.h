#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
enum AnimationState
{
	ATTACK_ANI,		//�ʏ�U��
	BIG_ATTACK_ANI0,//�ʏ�Ռ��g
	BIG_ATTACK_ANI1,//���ߍU��
	BIG_ATTACK_ANI2,//�����ߍU��
	DRAG_ANI		//��������
};
class Player :public GameObject
{
public:
	Player();
	~Player();
	void Init();  //������
	void Update();//�X�V
	void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)override;  //�`��
	void ResetJumpCnt()
	{
		jump_cnt = 0;
	}
	DirectX::XMFLOAT3 GetBoxSize()override;
private:
	Input& input = Input::GetInstance();
	std::unordered_map<AnimationState, std::shared_ptr<GameObject>> playerAnimations; // �v���C���[���Ǘ�����Q�[���I�u�W�F�N�g���X�g
	BoxCollider boxColl;
	float scrollVelocity = 1.3f;//�X�N���[���̑��x
	int jump_cnt=0;
	bool enterRelease = false;
	bool landing = false;    //�n�ʂɂ��邩
	bool new_landing = false;//�n�ʂɂ��邩
	bool old_landing = false;//�n�ʂɂ��邩
	int playerHP = 300;//hp
	bool damage_flg = false;//�_���[�W���󂯂���true
	const int invincibility_time =120;//���G����
	int invincibility_cnt = 0;
	int runUV_cnt = 0;

	int enterCnt = 0;
	int enterCnt_Old = 0;
	bool drag_n = 0;
	bool drag_o = 0;

	int m_Flame_cnt = 0;

	int changeAngleCnt = 0;//�p�x��ύX������J�E���g�J�n
	bool changeAngle = false;

	bool m_AttackFlg = false;
	int m_AttackCnt = 0;

	int m_UpdateAniFlame = 4;
};