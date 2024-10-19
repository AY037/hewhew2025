#include "Camera.h"
void Camera::Init()
{
	SetCameraPos(0, 0, -100);//カメラ初期位置
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
	// ビュー変換行列を作成
	DirectX::XMVECTOR viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	DirectX::XMVECTOR viewRot = GetRotationQuaternion(); // クォータニオンを取得
	DirectX::XMVECTOR viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// クォータニオンを回転行列に変換
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(viewRot);

	// 前方ベクトルを取得
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