#pragma once
#include"input.h"
#include"direct3d.h"
// プロジェクション変換行列設定
#define FOV (60.0f)
#define NEAR_CLIP (1.0f)
#define FAR_CLIP (400.0f)
class Camera
{
public:
	Camera() { cameraPos = { 0.0f, 0.0f, 0.0f };
	cameraAngle = { 0.0f, 0.0f, 0.0f };
	}
	~Camera(){}
	void Init();   //初期化
	void Update(const DirectX::XMFLOAT3& playerPos = { 0,0,0 },bool stop_flg=false); //更新   
	void EngineCameraUpdate(); //更新   
	void Uninit(); //終了

	DirectX::XMMATRIX SetViewMatrix();			   //プロジェクション変換行列のセット
	DirectX::XMMATRIX SetProjectionMatrix();	   //プロジェクション変換行列のセット
	DirectX::XMVECTOR GetRotationQuaternion();
	void SetCameraPos(float x, float y, float z);  //カメラ座標をセット
	DirectX::XMFLOAT3 GetCameraPos(void);		   //カメラ座標をゲット
	DirectX::XMFLOAT3 GetCameraAngle(void);		   //カメラ角度のゲッター

private:
	Input& input = Input::GetInstance();
	//カメラ座標
	DirectX::XMFLOAT3 cameraPos = { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 cameraAngle= { 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT2 mousePos = { 0.0f, 0.0f};
};
