#pragma once
#include "GameObject.h"
class RayCollider
{
public:
	RayCollider() {}
	~RayCollider() {}
	bool RayBoxCollider(const DirectX::XMFLOAT3& rayPos,const std::shared_ptr<GameObject> obj);// �_���l�p�`�̓����ɂ��邩�𔻒肷��֐�
	float CrossProduct(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);//�O�όv�Z
};

