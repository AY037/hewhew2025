#include "Camera.h"
#include "EventManager.h"
#include "GameManager.h"
#include <random>
float GenerateRandomInt(int min, int max) {
	// �����_���G���W�����������i1�񂾂����s�����j
	static std::random_device rd;  // �V�[�h���
	static std::mt19937 engine(rd());  // �����Z���k�E�c�C�X�^���g�p

	// ��l���z��ݒ�
	std::uniform_int_distribution<int> dist(min, max);

	// �����𐶐�
	int num = dist(engine);
	return num / 10.0f;
}

void Camera::Init()
{
#ifdef GUI_MODE
	SetCameraPos(0, 30, -200);//�J���������ʒu
#else
	SetCameraPos(0, 30, -200);//�J���������ʒu
#endif
	EventManager::GetInstance().AddListener("Vibration", [this]() { CameraVibration(); });

	playerPos = GameManager::GetInstance().GetPlayerPos();

	const DirectX::XMFLOAT2 adjust_cameraPos = { 80,40 };
	m_CameraPos.x = playerPos.x + adjust_cameraPos.x;
	m_CameraPos.y = playerPos.y + adjust_cameraPos.y;
}

void Camera::Update(const DirectX::XMFLOAT3& _playerPos, bool stop_flg)
{
	//�J������h�炷
	m_CameraPos.x -= vibrationVector.x;
	m_CameraPos.y -= vibrationVector.y;
	vibrationVector = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);//�h��̃��Z�b�g

	playerPos = _playerPos;

	float distanceX = oldPlayerPos.x - playerPos.x;
	if (-0.3f < distanceX && distanceX < 0.3f)
	{
		m_IsPlayerStop = true;
	}
	else
	{
		m_IsPlayerStop = false;
	}

	//�J�����O�Ƀv���C���[���o���ꍇ
	if (playerPos.x < m_CameraPos.x - 160)
	{
		m_IsPlayerStop = false;
	}

	const DirectX::XMFLOAT2 adjust_cameraPos = { 80,40 };
	if (!stop_flg)
	{
		//�J�����̃X�N���[��
		m_CameraPos.x += scrollVelocity;

		//x���̓J�������v���C���[�Ɉʒu�ɍ��킹��
		float targetPointX = playerPos.x + adjust_cameraPos.x;
		if (targetPointX - 1.0f > m_CameraPos.x || targetPointX + 1.0f < m_CameraPos.x)
		{
			//�v���C���[���X�N���[���ɂ��Ă��Ă���ꍇ�J�����𒲐�
			if(!m_IsPlayerStop)
			{
				m_CameraPos.x += (targetPointX > m_CameraPos.x) ? 1.0f : -1.0f;
			}
		}

		//y���͉�ʊO�ɂ������ꍇ���킹��
		if (playerPos.y< m_CameraPos.y - adjust_cameraPos.y || playerPos.y > m_CameraPos.y + adjust_cameraPos.y)
		{
			float targetPointY = playerPos.y + adjust_cameraPos.y;
			if (targetPointY - 1.0f > m_CameraPos.y || targetPointY + 1.0f < m_CameraPos.y)
			{
				m_CameraPos.y += (targetPointY > m_CameraPos.y) ? 1.0f : -1.0f;
			}
		}
	}
	else
	{
		const int vibration = 600;
		vibrationVector = DirectX::XMFLOAT3(GenerateRandomInt(-vibration, vibration), GenerateRandomInt(-vibration, vibration), 0.0f);
		m_CameraPos.x += vibrationVector.x;
		m_CameraPos.y += vibrationVector.y;
	}
	m_CameraPos.z = -160;

	oldPlayerPos = playerPos;

	GameManager::GetInstance().cameraPos = m_CameraPos;
}

void Camera::CameraVibration()
{
	const DirectX::XMFLOAT2 adjust_cameraPos = { 80,40 };
	if ((input.GetRightTriggerPress()) || (input.GetKeyPress(VK_SPACE)))
	{
		flame_cnt++;
		int vibration = 0;
		if (flame_cnt < 60)
		{
			vibration = 3;
		}
		if (60 <= flame_cnt && flame_cnt < 120)
		{
			vibration = 6;
		}
		if (flame_cnt >= 120)
		{
			vibration = 10;
		}

		vibrationVector = DirectX::XMFLOAT3(GenerateRandomInt(-vibration, vibration), GenerateRandomInt(-vibration, vibration), 0.0f);
		m_CameraPos.x += vibrationVector.x;
		m_CameraPos.y += vibrationVector.y;
	}
	if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
	{
		flame_cnt = 0;
		//cameraPos.x -= 50;
	}
}

void Camera::EngineCameraUpdate()
{
	if (input.GetKeyPress(VK_W))
	{
		m_CameraPos.y += 1;
	}
	if (input.GetKeyPress(VK_S))
	{
		m_CameraPos.y -= 1;
	}
	if (input.GetKeyPress(VK_A))
	{
		m_CameraPos.x -= 4;
	}
	if (input.GetKeyPress(VK_D))
	{
		m_CameraPos.x += 4;
	}
	m_CameraPos.z = -200;
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	// �r���[�ϊ��s����쐬
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z, 1.0f);
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
	m_CameraPos.x = x;
	m_CameraPos.y = y;
	m_CameraPos.z = z;
}

DirectX::XMFLOAT3 Camera::GetCameraPos(void)
{
	//�J�������W���Q�b�g
	return m_CameraPos;
}

DirectX::XMFLOAT3 Camera::GetCameraAngle(void)//�J�����p�x�̃Q�b�^�[
{
	return cameraAngle;
}