#pragma once
#include "Camera.h"
class Raycast
{
public:
	Raycast() {}
	~Raycast() {}
	//レイのワールド座標のｚ=0の時の値を返す
	DirectX::XMFLOAT3 Raycasting(DirectX::XMFLOAT2 mousePos,const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& protMat, DirectX::XMFLOAT3 cameraPos);//レイを作成し逆行列でワールド座標に変換
	DirectX::XMFLOAT3 RayIntersectsZPlane(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection);//レイがz=0の時のxとyを計算
};

