#include "Camera.h"
#include "EventManager.h"
#include <random>
float GenerateRandomInt(int min, int max) {
	// ランダムエンジンを初期化（1回だけ実行される）
	static std::random_device rd;  // シードを提供
	static std::mt19937 engine(rd());  // メルセンヌ・ツイスタを使用

	// 一様分布を設定
	std::uniform_int_distribution<int> dist(min, max);

	// 乱数を生成
	int num = dist(engine);
   	return num /10.0f;
}

void Camera::Init()
{
#ifdef GUI_MODE
	SetCameraPos(0, 30, -200);//カメラ初期位置
#else
	SetCameraPos(0, 30, -200);//カメラ初期位置
#endif
	EventManager::GetInstance().AddListener("Vibration", [this]() { CameraVibration(); });
}

void Camera::Update(const DirectX::XMFLOAT3& _playerPos , bool stop_flg)
{
	playerPos = _playerPos;
	const DirectX::XMFLOAT2 adjust_cameraPos = { 120,60 };
	if(stop_flg==false)
	{
		cameraPos.x = playerPos.x + adjust_cameraPos.x;
		cameraPos.y = adjust_cameraPos.y;
	}
	else
	{
		const int vibration=300;
		cameraPos.x = playerPos.x + adjust_cameraPos.x + GenerateRandomInt(-vibration, vibration);
		cameraPos.y = adjust_cameraPos.y + GenerateRandomInt(-vibration, vibration);
	}
	cameraPos.z = -160;
}

void Camera::CameraVibration()
{
	const DirectX::XMFLOAT2 adjust_cameraPos = { 120,60 };
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

		cameraPos.x = playerPos.x + adjust_cameraPos.x + GenerateRandomInt(-vibration, vibration);
		cameraPos.y = adjust_cameraPos.y + GenerateRandomInt(-vibration, vibration);
	}
	if ((input.GetRightTriggerRelease()) || (input.GetKeyRelease(VK_SPACE)))
	{
		flame_cnt = 0;
	}
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
	cameraPos.z = -200;
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	// ビュー変換行列を作成
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
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
	cameraPos.x = x;
	cameraPos.y = y;
	cameraPos.z = z;
}

DirectX::XMFLOAT3 Camera::GetCameraPos(void)
{
	//カメラ座標をゲット
	return cameraPos;
}

DirectX::XMFLOAT3 Camera::GetCameraAngle(void)//カメラ角度のゲッター
{
	return cameraAngle;
}