#include "Player.h"
#include "EventManager.h"
void Player::Init(TextureManager& _textureManager)
{
	Initialize("asset/char01.png",  _textureManager, 3, 4); //�v���C���[��������
	SetColor(1.0f, 1.0f, 1.0f, 1.0f);//�p�x��ݒ�
	for (auto& component : components)
	{
		component->Init(*this);
	}
}

void Player::Update(void)
{
	DirectX::XMFLOAT3 _pos = GetPos();
	DirectX::XMFLOAT3 _velocity = GetVelocity();


	if (input.GetKeyPress(VK_SPACE))
	{
		//�v���C���[�̃A�j���[�V����
		numU++;
		if (numU >= 3) {
			numU = 0;
		}

		_velocity.x = 2;
		SetVelocity(_velocity);
	}

	SetPos(_pos.x + _velocity.x, _pos.y + _velocity.y, _pos.z + _velocity.z);

	for (auto& component : components)
	{
		component->Update();
	}
}

void Player::Draw(void)
{
	//Render();      //�v���C���[��`��
}