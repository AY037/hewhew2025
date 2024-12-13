#include "Camera.h"
#include <random>
int GenerateRandomInt(int min, int max) {
	// ランダムエンジンを初期化（1回だけ実行される）
	static std::random_device rd;  // シードを提供
	static std::mt19937 engine(rd());  // メルセンヌ・ツイスタを使用

	// 一様分布を設定
	std::uniform_int_distribution<int> dist(min, max);

	// 乱数を生成
	return dist(engine);
}

void Camera::Init()
{
#ifdef GUI_MODE
	SetCameraPos(0, 30, -200);//カメラ初期位置
#else
	SetCameraPos(0, 0, -200);//カメラ初期位置
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