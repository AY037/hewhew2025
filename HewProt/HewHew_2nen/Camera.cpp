#include "Camera.h"
#include <random>
int GenerateRandomInt(int min, int max) {
	// �����_���G���W�����������i1�񂾂����s�����j
	static std::random_device rd;  // �V�[�h���
	static std::mt19937 engine(rd());  // �����Z���k�E�c�C�X�^���g�p

	// ��l���z��ݒ�
	std::uniform_int_distribution<int> dist(min, max);

	// �����𐶐�
	return dist(engine);
}

void Camera::Init()
{
#ifdef GUI_MODE
	SetCameraPos(0, 30, -200);//�J���������ʒu
#else
	SetCameraPos(0, 0, -200);//�J���������ʒu
#endif
}

void Camera::Update(const DirectX::XMFLOAT3& playerPos , bool stop_flg)
{
	if(stop_flg==false)
	{
		cameraPos.x = playerPos.x + 100;
		cameraPos.y = 30;
	}
	else
	{
		const int vibration=20;
		cameraPos.x = playerPos.x + 100+ GenerateRandomInt(-vibration, vibration);
		cameraPos.y = 30 + GenerateRandomInt(-vibration, vibration);
	}

	//mousePos = input.GetMousePos();
	//DirectX::XMFLOAT2 midPos;
	//cameraAngle.x = -(SCREEN_WIDTH / 2 - mousePos.x)/8;
	//cameraAngle.y = -(SCREEN_HEIGHT / 2 - mousePos.y)/8;
}

void Camera::EngineCameraUpdate()
{
	if (input.GetKeyPress(VK_W))
	{
		cameraPos.y++;
	}
	if (input.GetKeyPress(VK_S))
	{
		cameraPos.y--;
	}
	if (input.GetKeyPress(VK_A))
	{
		cameraPos.x -= 2;
	}
	if (input.GetKeyPress(VK_D))
	{
		cameraPos.x += 2;
	}
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	// �r���[�ϊ��s����쐬
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	DirectX::XMVECTOR viewRot = GetRotationQuaternion(); // �N�H�[�^�j�I�����擾
	DirectX::XMVECTOR viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// �N�H�[�^�j�I������]�s��ɕϊ�
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(viewRot);

	// �O���x�N�g�����擾�@�^�[�Q�b�g�����_
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