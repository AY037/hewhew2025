#pragma once
#include <memory>
#include <DirectXMath.h>
#include "Math.h"
class GameObject;

struct AABB
{
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int objectID = -1;
	// �I�u�W�F�N�g���m�̌�������
	bool intersects(const AABB& other) const {
		if (objectID != other.objectID)
		{
			if (right >= other.left && left <= other.right &&
				top >= other.bottom && bottom <= other.top) {
				return true;
			}
		}
		return false;
	}

	// AABB�𓝍�����֐�
	static AABB merge(const AABB& a, const AABB& b) {
		AABB aabb;
		aabb.left = math::Min(a.left, b.left);
		aabb.top = math::Max(a.top, b.top);
		aabb.right = math::Max(a.right, b.right);
		aabb.bottom = math::Min(a.bottom, b.bottom);
		return aabb;
	}

	// AABB�̕\�ʐρi�R�X�g�v�Z�Ŏg�p�j
	float surfaceArea() const {
		float dx = right - left;
		float dy = top - bottom;
		return 2.0f * (dx * dy);
	}

	//AABB���X�V����Ă��邩�m�F
	bool contains(const AABB& other)
	{
		if ((left == other.left && top == other.top) && (right == other.right && top == other.top))
		{
			return true;
		}

		return false;
	}
};

struct OBB {
	DirectX::XMFLOAT2 center;   // ���S���W
	DirectX::XMFLOAT2 halfSize; // �e���̔����̃T�C�Y
	DirectX::XMFLOAT2 axes[2];  // �e���̕����x�N�g���i���K���ς݁j


	DirectX::XMFLOAT2 GetDirection2D(GameObject& obj);

	OBB& GetObb(GameObject& obj);

	// �x�N�g���̓���
	float Dot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);

	// �x�N�g���̐�Βl
	float AbsDot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b);

	bool IntersectsWithNormal(std::shared_ptr<GameObject>& obj1, std::shared_ptr<GameObject>& obj2, DirectX::XMFLOAT2& normal, bool useResolve = true);

	//�d�Ȃ�̉���
	void ResolveOverlap(GameObject& obj1, GameObject& obj2, const DirectX::XMFLOAT2& normal, float overlap);
};

