#pragma once
#include "Camera.h"
class Raycast
{
public:
	Raycast() {}
	~Raycast() {}
	//���C�̃��[���h���W�̂�=0�̎��̒l��Ԃ�
	DirectX::XMFLOAT3 Raycasting(DirectX::XMFLOAT2 mousePos,const DirectX::XMMATRIX& viewMat, const DirectX::XMMATRIX& protMat, DirectX::XMFLOAT3 cameraPos);//���C���쐬���t�s��Ń��[���h���W�ɕϊ�
	DirectX::XMFLOAT3 RayIntersectsZPlane(const DirectX::XMVECTOR& rayOrigin, const DirectX::XMVECTOR& rayDirection);//���C��z=0�̎���x��y���v�Z
};

