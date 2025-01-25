#include "Math.h"
using namespace DirectX::SimpleMath;
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


	DirectX::SimpleMath::Vector3 GetRotateVec(float x, float y, float z, float radAngle,int axis)
	{
		if (axis > 2)
		{
			axis = Z;
		}
		//��]���̎w��
		Vector3 rotationAxis;
		switch (axis)
		{
		case X:
			rotationAxis = Vector3(1.0f, 0.0f, 0.0f);
			break;
		case Y:
			rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
			break;
		case Z:
			rotationAxis = Vector3(0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}

		Vector3 vec = Vector3(x, y, z);

		// ��]�s��̍쐬
		Matrix rotationMatrix = Matrix::CreateFromAxisAngle(rotationAxis, radAngle);

		// �x�N�g������]
		Vector3 rotatedVec = Vector3::Transform(vec, rotationMatrix);

		return rotatedVec;
	}
}

float Math::DistanceFloat3(const DirectX::XMFLOAT3& a,const DirectX::XMFLOAT3& b)
{
    // �������擾
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}
