#include "Math.h"
using namespace DirectX;
namespace math
{
	float Max(float i, float j)
	{
		if (i >= j)return i;
		if (j > i)return j;
	}
	float Min(float i, float j)
	{
		if (i >= j)return j;
		if (j > i)return i;
	}
}
float Math::DistanceFloat3(const XMFLOAT3& a,const XMFLOAT3& b)
{
    // ‹——£‚ðŽæ“¾
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
