#include "ColliderStructure.h"
#include "GameObject.h"
DirectX::XMFLOAT2 OBB::GetDirection2D(GameObject& obj) {
	float radians = DirectX::XMConvertToRadians(obj.GetAngle()); // �x�����W�A���ɕϊ�
	return DirectX::XMFLOAT2(std::cos(radians), std::sin(radians));
}

OBB& OBB::GetObb(GameObject& obj) {
	OBB obb;
	DirectX::XMFLOAT3 pos = obj.GetPos();
	DirectX::XMFLOAT3 size = obj.GetBoxSize();
	obb.center = { pos.x, pos.y };
	obb.halfSize = { size.x / 2, size.y / 2 };

	// ��]���̕����x�N�g��
	DirectX::XMFLOAT2 direction = GetDirection2D(obj);

	// �e����ݒ�i���K���ς݁j
	obb.axes[0] = direction; // ��]���̕���
	obb.axes[1] = { -direction.y, direction.x }; // �������鎲

	return obb;
}
float OBB::Dot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) {
	return a.x * b.x + a.y * b.y;
}

float OBB::AbsDot(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) {
	return std::fabs(Dot(a, b));
}

bool OBB::IntersectsWithNormal(std::shared_ptr<GameObject>& obj1, std::shared_ptr<GameObject>& obj2, DirectX::XMFLOAT2& outNormal,bool useResolve) {
	using namespace DirectX;

	// OBB�����擾
	OBB obb1 = GetObb(dynamic_cast<GameObject&>(*obj1));
	OBB obb2 = GetObb(dynamic_cast<GameObject&>(*obj2));

	// ���S�_�̍����i�I�u�W�F�N�g�Ԃ̋����x�N�g���j
	XMFLOAT2 T = {
		obb2.center.x - obb1.center.x,
		obb2.center.y - obb1.center.y,
	};

	float minOverlap = FLT_MAX; // �ŏ��̃I�[�o�[���b�v�l
	XMFLOAT2 bestAxis = {};     // �@�����

	// ���ׂĂ̎������� (obb1 �� obb2 �� 2 ������)
	for (int i = 0; i < 2; ++i) {
		// obb1 �̎�
		const XMFLOAT2& axis = obb1.axes[i];

		// obb1 �̎��ɑ΂��锼�a
		float r1 = obb1.halfSize.x * std::fabs(Dot(axis, obb1.axes[0])) +
			obb1.halfSize.y * std::fabs(Dot(axis, obb1.axes[1]));

		// obb2 �̎��ɑ΂��锼�a
		float r2 = obb2.halfSize.x * std::fabs(Dot(axis, obb2.axes[0])) +
			obb2.halfSize.y * std::fabs(Dot(axis, obb2.axes[1]));

		// ����ł̒��S�ԋ���
		float distance = std::fabs(Dot(T, axis));

		// �I�[�o�[���b�v�ʂ��v�Z
		float overlap = r1 + r2 - distance;

		if (overlap < 0) {
			return false; // �Փ˂��Ă��Ȃ�
		}

		// �ŏ��I�[�o�[���b�v�����L�^
		if (overlap < minOverlap) {
			minOverlap = overlap;
			bestAxis = axis;
		}
	}

	// obb2 �̎��ɂ��Ă����l�ɏ���
	for (int i = 0; i < 2; ++i) {
		const XMFLOAT2& axis = obb2.axes[i];

		float r1 = obb1.halfSize.x * std::fabs(Dot(axis, obb1.axes[0])) +
			obb1.halfSize.y * std::fabs(Dot(axis, obb1.axes[1]));

		float r2 = obb2.halfSize.x * std::fabs(Dot(axis, obb2.axes[0])) +
			obb2.halfSize.y * std::fabs(Dot(axis, obb2.axes[1]));

		float distance = std::fabs(Dot(T, axis));
		float overlap = r1 + r2 - distance;

		if (overlap < 0) {
			return false; // �Փ˂��Ă��Ȃ�
		}

		if (overlap < minOverlap) {
			minOverlap = overlap;
			bestAxis = axis;
		}
	}

	// �@���x�N�g���Ƃ��čŏ��I�[�o�[���b�v�̎���Ԃ�
	outNormal = bestAxis;

	if(useResolve)
	{
		//Player��Ground�ȊO����
		if (obj1->GetObjTypeName() == "Player" || obj2->GetObjTypeName() == "Player")
		{
			if (obj1->GetObjTypeName() == "Ground" || obj2->GetObjTypeName() == "Ground")
			{
				ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
			}
		}
		else
		{
			if (obj1->GetObjTypeName() == "DragSword" || obj2->GetObjTypeName() == "DragSword")
			{
				if (obj1->GetObjTypeName() == "Debri" || obj2->GetObjTypeName() == "Debri")
				{
					ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
				}
			}
			else
			{
				ResolveOverlap(dynamic_cast<GameObject&>(*obj1), dynamic_cast<GameObject&>(*obj2), bestAxis, minOverlap);
			}
		}
	}
	return true; // �Փ˂��Ă���
}

void OBB::ResolveOverlap(GameObject& obj1, GameObject& obj2, const DirectX::XMFLOAT2& normal, float overlap)
{
	using namespace DirectX;

	DirectX::XMFLOAT3 pos1 = obj1.GetPos();
	DirectX::XMFLOAT3 pos2 = obj2.GetPos();

	// ���S�Ԃ̋����x�N�g��
	XMFLOAT2 T = {
		pos2.x - pos1.x,
		pos2.y - pos1.y
	};

	// �����������o���������m�� (T �Ɩ@���̓��ςŊm�F)
	float direction = Dot(T, normal) < 0.0f ? -1.0f : 1.0f;
	XMFLOAT2 correction = {
		direction * normal.x * overlap,
		direction * normal.y * overlap
	};

	// �I�u�W�F�N�g���C��
	XMFLOAT2 position1 = { pos1.x,pos1.y };
	XMFLOAT2 position2 = { pos2.x,pos2.y };

	//�����v���O����
	//���������ꍇ���������I�u�W�F�N�g�̂ݏd�Ȃ����
	if (obj1.GetObjTypeName() != "Sword")
	{
		position1.x -= correction.x;
		position1.y -= correction.y;
	}
	else
	{
		correction.x *= 2;
		correction.y *= 2;
	}
	if (obj2.GetObjTypeName() != "Sword")
	{
		position2.x += correction.x;
		position2.y += correction.y;
	}
	else
	{
		position1.x -= correction.x;
		position1.y -= correction.y;
	}

	// �V�����ʒu��ݒ�
	// ���I�I�u�W�F�N�g�݈̂ړ�
	if (obj1.GetObjectType() == true)
	{
		obj1.SetPos(position1.x, position1.y, 0);
	}
	if (obj2.GetObjectType() == true)
	{
		obj2.SetPos(position2.x, position2.y, 0);
	}
}