#pragma once
#include "GameObject.h"
#include "BoxCollider.h"
enum AnimationState
{
	ATTACK_ANI,		//�ʏ�U��
	BIG_ATTACK_ANI1,//���ߍU��
	BIG_ATTACK_ANI2,//�����ߍU��
	DRAG_ANI		//��������
};
class Player :public GameObject
{
public:
	Player(){
		AddComponent("Rigidbody");
		SetObjTypeName("Player");
		isRigidbody = true;
	}
	~Player() { playerAnimations.clear(); }
	void Init(TextureManager& _textureManager);  //������
	void Update();//�X�V
	void Draw(DirectX::XMMATRIX& _vm, DirectX::XMMATRIX& _pm)override;  //�`��
	void ResetJumpCnt()
	{
		jump_cnt = 0;
	}
private:
	Input& input = Input::GetInstance();
	std::unordered_map<AnimationState, std::shared_ptr<GameObject>> playerAnimations; // �v���C���[���Ǘ�����Q�[���I�u�W�F�N�g���X�g
	TextureManager* textureManager=nullptr;
	BoxCollider boxColl;
	const float scrollVelocity = 1.3f;//�X�N���[���̑��x
	int jump_cnt=0;
	bool enterRelease = false;
	bool landing = false;    //�n�ʂɂ��邩
	bool new_landing = false;//�n�ʂɂ��邩
	bool old_landing = false;//�n�ʂɂ��邩
	int playerHP = 5;//hp
	bool damage_flg = false;//�_���[�W���󂯂���true
	const int invincibility_time =120;//���G����
	int invincibility_cnt = 0;
	int runUV_cnt = 0;

	int enterCnt = 0;
	bool drag_n = 0;
	bool drag_o = 0;
};