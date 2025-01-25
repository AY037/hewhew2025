#pragma once
#include <DirectXMath.h>
//�V���O���g��
class GameManager
{
public:
	GameManager();
	~GameManager();
	static GameManager& GetInstance()
	{
		static GameManager instance;
		return instance;
	}
	//�v���C���[���W�A�G�l�~�[�̃v���C���[���W�擾�p
	void SetPlayerPos(const DirectX::XMFLOAT3& pPos);
	DirectX::XMFLOAT3 GetPlayerPos() const;
	//�v���C���[HP�A�c��HP�`��p
	void SetPlayerHP(const int pHP);
	int GetPlayerHP() const;
	int dragCharging = 0;
	bool dragSwordHit = false;//�������茕���������Ă��邩
	DirectX::XMFLOAT3 dragSwordPos = DirectX::XMFLOAT3(0.0f,0.0f,0.0f);//�������茕�̍��W
	int score = 0;//�X�R�A
	DirectX::XMFLOAT3 cameraPos = { 0.0f,0.0f,0.0f };
private:
	DirectX::XMFLOAT3 playerPos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	int playerHP = 0;
};

