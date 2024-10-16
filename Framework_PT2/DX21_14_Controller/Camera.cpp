#include "Camera.h"
#include <cstdio>
#include "iostream"
void Camera::Init()
{
	SetCameraPos(0, 0, -100);//�J���������ʒu
	// �R���\�[�������蓖�Ă�
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
}

void Camera::Update(void)
{
	input.Update();

	if (input.GetKeyPress(VK_UP))
	{
		cameraPos.z++;
	}
	if (input.GetKeyPress(VK_DOWN))
	{
		cameraPos.z--;
	}
	if (input.GetKeyPress(VK_RIGHT))
	{
		cameraPos.x++;
	}
	if (input.GetKeyPress(VK_LEFT))
	{
		cameraPos.x--;
	}
	std::cout << cameraPos.x << "\t" << cameraPos.x << std::endl;//updaate�ɓ����
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	//�r���[�ϊ��s����쐬
	DirectX::XMVECTOR viewPos, viewRot, viewUp;
	viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	viewRot = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookToLH(viewPos, viewRot, viewUp);
	return viewMat;
}
DirectX::XMMATRIX Camera::SetProjectionMatrix()
{
	// �v���W�F�N�V�����ϊ��s����쐬
	//cb.matrixProj = DirectX::XMMatrixOrthographicLH( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);//���𓊉e�s�� ���ߊ��Ȃ�
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	DirectX::XMMATRIX projectionMat = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(FOV),   // ����p
		aspectRatio,                        // �A�X�y�N�g��
		NEAR_CLIP,                          // �j�A�N���b�v
		FAR_CLIP                            // �t�@�[�N���b�v
		//�J��������̋�����NEAR_CLIP�ȏ㗣��Ă���I�u�W�F�N�g��`��FAR_CLIP�͂��̋t
	);
	return projectionMat;
}
void Camera::Uninit(void)
{
	//Uninit();
}

void Camera::SetCameraPos(float x, float y, float z)
{
	//�J�������W���Z�b�g����
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;
}

DirectX::XMFLOAT3 Camera::GetCameraPos(void)
{
	//�J�������W���Q�b�g
	return cameraPos;
}
