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
private:
	DirectX::XMFLOAT3 playerPos;
	int playerHP = 0;
};

