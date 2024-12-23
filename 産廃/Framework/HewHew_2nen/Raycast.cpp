#include "Raycast.h"
#include "direct3d.h"
using namespace DirectX;

DirectX::XMFLOAT3 Raycast::Raycasting(DirectX::XMFLOAT2 mousePos, const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& projMat,DirectX::XMFLOAT3 cameraPos)
{
	// �X�N���[�����W��[-1, 1]�͈̔͂ɕϊ�
	float x = mousePos.x / (SCREEN_WIDTH / 2);
	float y = -mousePos.y / (SCREEN_HEIGHT / 2);
	float z = 1.0f;  // ���C�̐[�x�B�ʏ�A1.0f�őO���Ɍ�����
	DirectX::XMFLOAT3 rayClipSpace = { x, y, z };

	// �N���b�v��� �� �r���[���
	DirectX::XMMATRIX inverseProjMat = DirectX::XMMatrixInverse(nullptr, projMat);
	DirectX::XMVECTOR rayView = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&rayClipSpace), inverseProjMat);

	// �r���[��� �� ���[���h���
	DirectX::XMMATRIX inverseViewMat = DirectX::XMMatrixInverse(nullptr, viewMat);
	DirectX::XMVECTOR rayWorld = DirectX::XMVector3TransformNormal(rayView, inverseViewMat);

	// ���K��
	rayWorld = DirectX::XMVector3Normalize(rayWorld);

	// Z���ʂƂ̌�_���v�Z
	DirectX::XMVECTOR rayOrigin = DirectX::XMLoadFloat3(&cameraPos);
	return RayIntersectsZPlane(rayOrigin, rayWorld);
}

//���C��z=0�̎���x��y���v�Z
DirectX::XMFLOAT3 Raycast::RayIntersectsZPlane(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection)
{
	// rayDirection��z������0�łȂ����Ƃ��m�F
	float rayDirectionZ = DirectX::XMVectorGetZ(rayDirection);
	if (rayDirectionZ == 0.0f)
	{
		// z�����ɐi�܂Ȃ����C�͌������Ȃ�
		return DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	// z=0�Ō�������t���v�Z
	float t = -DirectX::XMVectorGetZ(rayOrigin) / rayDirectionZ;//rayOrigin�J�������W���牓�����邽�߃}�C�i�X

	// �����_�̍��W���v�Z
	DirectX::XMVECTOR intersection = rayOrigin + t * rayDirection;

	// XMVECTOR��XMFLOAT3�ɕϊ����ĕԂ�
	DirectX::XMFLOAT3 intersectionPoint;
	DirectX::XMStoreFloat3(&intersectionPoint, intersection);
	return intersectionPoint;
}
