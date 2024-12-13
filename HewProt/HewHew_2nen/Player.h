#pragma once
#include "GameObject.h"
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
		AddComponent("BoxCollider");
		isRigidbody = true;
	}
	~Player() {}
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
	const float scrollVelocity = 1.3f;//�X�N���[���̑��x
	int jump_cnt=0;
	bool enterRelease = false;
};