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
	ATTACK_ANI=0,		//�ʏ�U��
	BIG_ATTACK_ANI0,//�ʏ�Ռ��g
	BIG_ATTACK_ANI1,//���ߍU��
	BIG_ATTACK_ANI2,//�����ߍU��
	DRAG_ANI		//��������
};

enum LandingState
{
	SLIDING,//������蒆
	LANDING,//������˒�
	NORMAL, //�ʏ���
};

struct Land {
	LandingState landState=NORMAL;
	bool changeTexture = false;
	float posX =0.0f;//�p�x���O�ɖ߂�܂ł̋���
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

	void ResetTexture();//�e�N�X�`����ʏ��Ԃ�
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

	bool m_AttackFlg = false;
	int m_AttackCnt = 0;

	int m_UpdateAniFlame = 4;

	bool drawDragAni = false;//��������̕`��
	int slopeId = -1;//�����Ă���id
	Land land;
	PlayerAnimationState playerAnimationState;
};