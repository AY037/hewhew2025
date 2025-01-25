#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>

namespace math
{
	enum
	{
		X=0,
		Y,
		Z
	};

	float Max(float i, float j);
	float Min(float i, float j);

	DirectX::SimpleMath::Vector3 GetRotateVec(float x, float y, float z, float radAngle,int axis);
}
class Math
{
public:
	static float DistanceFloat3(const DirectX::XMFLOAT3& pointA, const DirectX::XMFLOAT3& pointB);
};

