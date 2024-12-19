#include "BigAttackAnimation1.h"
#include "EventManager.h"

void BigAttackAnimation1::Init(TextureManager& _textureManager)
{
	Initialize(GetObjectTexName(), _textureManager, 4, 2); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�F��ݒ�
	numU = 0;
	numV = 0;
}

void BigAttackAnimation1::Update(void)
{
	animation_cnt++;
	if (animation_cnt % 3==0)
	{
		numU++;
	}
	if (numU == 4 && numV == 0) {
		EventManager::GetInstance().SendEvent("deleteSword");
	}
	if (numU == 4) {
		numV = 1;
		numU = 0;
	}
	DirectX::XMFLOAT3 _pos = GetPos();

	SetPos(_pos.x + 3.0f, _pos.y + 1.0f, 0);
}

void BigAttackAnimation1::Draw(void)
{
	//Render();      //�v���C���[��`��
}