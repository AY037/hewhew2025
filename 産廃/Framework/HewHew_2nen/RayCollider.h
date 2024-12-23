#pragma once
#include "GameObject.h"
class RayCollider
{
public:
	RayCollider() {}
	~RayCollider() {}
	bool RayBoxCollider(const DirectX::XMFLOAT3& rayPos,const std::shared_ptr<GameObject> obj);// 点が四角形の内部にあるかを判定する関数
	float CrossProduct(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);//外積計算
};

