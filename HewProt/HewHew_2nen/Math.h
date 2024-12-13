#pragma once
#include <DirectXMath.h>
namespace math
{
	float Max(float i, float j);
	float Min(float i, float j);
}
class Math
{
public:
	static float DistanceFloat3(const DirectX::XMFLOAT3& pointA, const DirectX::XMFLOAT3& pointB);
};

