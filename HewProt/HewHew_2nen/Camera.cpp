#include "Camera.h"
#include "EventManager.h"
#include "GameManager.h"
#include <random>
float GenerateRandomInt(int min, int max) {
	// ランダムエンジンを初期化（1回だけ実行される）
	static std::random_device rd;  // シードを提供
	static std::mt19937 engine(rd());  // メルセンヌ・ツイスタを使用

	// 一様分布を設定
	std::uniform_int_distribution<int> dist(min, max);

	// 乱数を生成
	int num = dist(engine);
	return num / 10.0f;
}

void Camera::Init()
{
#ifdef GUI_MODE
	SetCameraPos(0, 30, -200);//カメラ初期位置
#else
	SetCameraPos(0, 30, -200);//カメラ初期位置
#endif
	EventManager::GetInstance().AddListener("Vibration", [this]() { CameraVibration(); });

	playerPos = GameManager::GetInstance().GetPlayerPos();

	const DirectX::XMFLOAT2 adjust_cameraPos = { 80,40 };
	m_CameraPos.x = playerPos.x + adjust_cameraPos.x;
	m_CameraPos.y = playerPos.y + adjust_cameraPos.y;
}

void Camera::Update(const DirectX::XMFLOAT3& _playerPos, bool stop_flg)
{
	//カメラを揺らす
	m_CameraPos.x -= vibrationVector.x;
	m_CameraPos.y -= vibrationVector.y;
	vibrationVector = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);//揺れのリセット

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

	//カメラ外にプレイヤーが出た場合
	if (playerPos.x < m_CameraPos.x - 160)
	{
		m_IsPlayerStop = false;
	}

	const DirectX::XMFLOAT2 adjust_cameraPos = { 80,40 };
	if (!stop_flg)
	{
		//カメラのスクロール
		m_CameraPos.x += scrollVelocity;

		//x軸はカメラをプレイヤーに位置に合わせる
		float targetPointX = playerPos.x + adjust_cameraPos.x;
		if (targetPointX - 1.0f > m_CameraPos.x || targetPointX + 1.0f < m_CameraPos.x)
		{
			//プレイヤーがスクロールについてきている場合カメラを調整
			if(!m_IsPlayerStop)
			{
				m_CameraPos.x += (targetPointX > m_CameraPos.x) ? 1.0f : -1.0f;
			}
		}

		//y軸は画面外にいった場合合わせる
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
	// ビュー変換行列を作成
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(m_CameraPos.x, m_CameraPos.y, m_CameraPos.z, 1.0f);
	DirectX::XMVECTOR viewRot = GetRotationQuaternion(); // クォータニオンを取得
	DirectX::XMVECTOR viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// クォータニオンを回転行列に変換
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(viewRot);

	// 前方ベクトルを取得　ターゲット注視点
	DirectX::XMVECTOR forward = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);

	// ビュー行列を作成
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookToLH(viewPos, forward, viewUp);
	return viewMat;
}

DirectX::XMMATRIX Camera::SetProjectionMatrix()
{
	// プロジェクション変換行列を作成
	//cb.matrixProj = DirectX::XMMatrixOrthographicLH( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 3.0f);//正交投影行列 遠近感なし
	float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
	DirectX::XMMATRIX projectionMat = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(FOV),   // 視野角
		aspectRatio,                        // アスペクト比
		NEAR_CLIP,                          // ニアクリップ
		FAR_CLIP                            // ファークリップ
		//カメラからの距離がNEAR_CLIP以上離れているオブジェクトを描画FAR_CLIPはその逆
	);
	return projectionMat;
}
DirectX::XMVECTOR Camera::GetRotationQuaternion()
{
	// Yaw（水平回転）、Pitch（垂直回転）からクォータニオンを作成
	return DirectX::XMQuaternionRotationRollPitchYaw(
		DirectX::XMConvertToRadians(cameraAngle.y), // ピッチ
		DirectX::XMConvertToRadians(cameraAngle.x), // ヨー
		1.0f                                         // ロール
	);
}
void Camera::Uninit(void)
{
	//Uninit();
}

void Camera::SetCameraPos(float x, float y, float z)
{
	//カメラ座標をセットする
	m_CameraPos.x = x;
	m_CameraPos.y = y;
	m_CameraPos.z = z;
}

DirectX::XMFLOAT3 Camera::GetCameraPos(void)
{
	//カメラ座標をゲット
	return m_CameraPos;
}

DirectX::XMFLOAT3 Camera::GetCameraAngle(void)//カメラ角度のゲッター
{
	return cameraAngle;
}