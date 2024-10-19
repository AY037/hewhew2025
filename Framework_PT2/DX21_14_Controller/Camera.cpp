#include "Camera.h"
void Camera::Init()
{
	SetCameraPos(0, 0, -100);//�J���������ʒu
}

void Camera::Update(void)
{
	mousePos = input.GetMousePos();
	if (input.GetKeyPress(VK_W))
	{
		cameraPos.z++;
	}
	if (input.GetKeyPress(VK_S))
	{
		cameraPos.z--;
	}
	if (input.GetKeyPress(VK_A))
	{
		cameraPos.x--;
	}
	if (input.GetKeyPress(VK_D))
	{
		cameraPos.x++;
	}
	DirectX::XMFLOAT2 midPos;
	cameraAngle.x = -(SCREEN_WIDTH / 2 - mousePos.x)/8;
	cameraAngle.y = -(SCREEN_HEIGHT / 2 - mousePos.y)/8;
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	// �r���[�ϊ��s����쐬
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	DirectX::XMVECTOR viewRot = GetRotationQuaternion(); // �N�H�[�^�j�I�����擾
	DirectX::XMVECTOR viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �N�H�[�^�j�I������]�s��ɕϊ�
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(viewRot);

	// �O���x�N�g�����擾
	DirectX::XMVECTOR forward = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);

	// �r���[�s����쐬
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookToLH(viewPos, forward, viewUp);
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
DirectX::XMVECTOR Camera::GetRotationQuaternion()
{
	// Yaw�i������]�j�APitch�i������]�j����N�H�[�^�j�I�����쐬
	return DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMConvertToRadians(cameraAngle.y), // �s�b�`
		DirectX::XMConvertToRadians(cameraAngle.x), // ���[
		1.0f                                         // ���[��
	);
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

DirectX::XMFLOAT3 Camera::GetCameraAngle(void)//�J�����p�x�̃Q�b�^�[
{
	return cameraAngle;
}