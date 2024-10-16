#include "Camera.h"
#include <cstdio>
#include "iostream"
void Camera::Init()
{
	SetCameraPos(0, 0, -100);//カメラ初期位置
	// コンソールを割り当てる
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
	std::cout << cameraPos.x << "\t" << cameraPos.x << std::endl;//updaateに入れる
}

DirectX::XMMATRIX Camera::SetViewMatrix()
{
	//ビュー変換行列を作成
	DirectX::XMVECTOR viewPos, viewRot, viewUp;
	viewPos = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
	viewRot = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	viewUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookToLH(viewPos, viewRot, viewUp);
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
