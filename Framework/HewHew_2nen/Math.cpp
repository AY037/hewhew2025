#include "Math.h"
using namespace DirectX;
float Math::DistanceFloat3(const XMFLOAT3& a,const XMFLOAT3& b)
{
    // �������擾
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}