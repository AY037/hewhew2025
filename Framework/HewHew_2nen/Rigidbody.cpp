#include "Rigidbody.h"
#include "GameObject.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
Rigidbody::Rigidbody()
{
}
void Rigidbody::Init(GameObject& obj)
{
	this->m_obj = &obj; // �A�h���X��ێ�����
	this->m_obj->GetPhysicsEventManager().AddonCollisionEvent(
		[this](GameObject& other) {this->ResolveCollision(other); });
}
void Rigidbody::Update()
{
	GameObject& obj = *static_cast<GameObject*>(this->m_obj);
	XMFLOAT3 velocity = obj.GetVelocity();
	//�x�N�g���ɕϊ�
	XMVECTOR velocity_vec = XMLoadFloat3(&velocity);


	XMVECTOR gravity_vec = XMLoadFloat3(&gravity);
	XMVECTOR airResi_vec = -velocity_vec * airResistance;
	velocity_vec += gravity_vec;
	velocity_vec += airResi_vec;


	XMStoreFloat3(&velocity, velocity_vec);

	obj.SetVelocity(velocity);
}

void Rigidbody::ResolveCollision(GameObject& other) {

	GameObject& obj = *this->m_obj;



	if (other.GetObjectType() == true)
	{
		DynamicCollision(obj, other);
	}
	else
	{
	}
	StaticCollision(obj, other);
}
void Rigidbody::ResolveOverlapStatic(GameObject& obj1, GameObject& obj2, float buffer)
{
	XMFLOAT3 obj1Pos = obj1.GetPos();
	XMFLOAT3 obj2Pos = obj2.GetPos();

	XMFLOAT3 obj1Size = obj1.GetSize();
	XMFLOAT3 obj2Size = obj2.GetSize();

	struct AABB
	{
		float minX, minY, maxX, maxY;
	};

	AABB aabb1 = {
		obj1Pos.x - (obj1Size.x / 2),
		obj1Pos.y - (obj1Size.y / 2),
		obj1Pos.x + (obj1Size.x / 2),
		obj1Pos.y + (obj1Size.y / 2),
	};

	AABB aabb2 = {
	 obj2Pos.x - (obj2Size.x / 2),
	 obj2Pos.y - (obj2Size.y / 2),
	 obj2Pos.x + (obj2Size.x / 2),
	 obj2Pos.y + (obj2Size.y / 2),
	};

	float overlapX = Min(aabb1.maxX, aabb2.maxX) - Max(aabb1.minX, aabb2.minX);
	float overlapY = Min(aabb1.maxY, aabb2.maxY) - Max(aabb1.minY, aabb2.minY);


	if (overlapX < overlapY) {
		// X�������̏d�Ȃ肪�傫���ꍇ
		float correctionX = overlapX + buffer;
		if (obj1Pos.x < obj2Pos.x) {
			// obj1��obj2�̍����ɂ���ꍇ
			obj1Pos.x -= correctionX;
		}
		else {
			// obj1��obj2�̉E���ɂ���ꍇ
			obj1Pos.x += correctionX;
		}
	}
	else {
		// Y�������̏d�Ȃ肪�傫���ꍇ
		float correctionY = overlapY + buffer;
		if (obj1Pos.y < obj2Pos.y) {
			// obj1��obj2�̉����ɂ���ꍇ
			obj1Pos.y -= correctionY;
		}
		else {
			// obj1��obj2�̏㑤�ɂ���ꍇ
			obj1Pos.y += correctionY;
		}
	}
	obj1.SetPos(obj1Pos.x, obj1Pos.y, obj1Pos.z);
	/*DirectX::XMFLOAT3 reset = { 0,0,0 };
	obj1.SetVelocity(reset);*/
	obj1.GetPhysicsEventManager().SendupdateAAABBEvent(obj1.GetObjID());
}
void Rigidbody::ResolveOverlapDynamic(GameObject& obj1, GameObject& obj2, float buffer) {
	XMFLOAT3 obj1Pos = obj1.GetPos();
	XMFLOAT3 obj2Pos = obj2.GetPos();

	XMFLOAT3 obj1Size = obj1.GetSize();
	XMFLOAT3 obj2Size = obj2.GetSize();

	struct AABB {
		float minX, minY, maxX, maxY;
	};

	AABB aabb1 = {
		obj1Pos.x - (obj1Size.x / 2),
		obj1Pos.y - (obj1Size.y / 2),
		obj1Pos.x + (obj1Size.x / 2),
		obj1Pos.y + (obj1Size.y / 2),
	};

	AABB aabb2 = {
		obj2Pos.x - (obj2Size.x / 2),
		obj2Pos.y - (obj2Size.y / 2),
		obj2Pos.x + (obj2Size.x / 2),
		obj2Pos.y + (obj2Size.y / 2),
	};

	float overlapX = Min(aabb1.maxX, aabb2.maxX) - Max(aabb1.minX, aabb2.minX);
	float overlapY = Min(aabb1.maxY, aabb2.maxY) - Max(aabb1.minY, aabb2.minY);

	XMFLOAT3 correctionVec = { 0, 0, 0 }; // �C���x�N�g��
	float correctionMagnitude;

	if (overlapX < overlapY) {
		// X�������̏d�Ȃ������
		correctionMagnitude = (overlapX + buffer) / 2;
		correctionVec.x = (obj1Pos.x < obj2Pos.x) ? -correctionMagnitude : correctionMagnitude;
	}
	else {
		// Y�������̏d�Ȃ������
		correctionMagnitude = (overlapY + buffer) / 2;
		correctionVec.y = (obj1Pos.y < obj2Pos.y) ? -correctionMagnitude : correctionMagnitude;
	}

	// �|�W�V�����C��
	obj1Pos.x += correctionVec.x;
	obj1Pos.y += correctionVec.y;
	obj2Pos.x -= correctionVec.x;
	obj2Pos.y -= correctionVec.y;

	obj1.SetPos(obj1Pos.x, obj1Pos.y, obj1Pos.z);
	obj2.SetPos(obj2Pos.x, obj2Pos.y, obj2Pos.z);

	// ���͂Ɋ�Â����x�v�Z
	XMFLOAT3 obj1Vel = obj1.GetVelocity();
	XMFLOAT3 obj2Vel = obj2.GetVelocity();

	XMFLOAT3 relativeVel = {
		obj2Vel.x - obj1Vel.x,
		obj2Vel.y - obj1Vel.y,
		obj2Vel.z - obj1Vel.z,
	};

	float pressure = correctionMagnitude/10; // �C���ʂ����͂Ƃ��Ĉ���

	// ���Α��x�𒲐����Ĕ������V�~�����[�g
	obj1Vel.x -= pressure * correctionVec.x;
	obj1Vel.y -= pressure * correctionVec.y;
	obj2Vel.x += pressure * correctionVec.x;
	obj2Vel.y += pressure * correctionVec.y;

	obj1.SetVelocity(obj1Vel);
	obj2.SetVelocity(obj2Vel);

	// AABB�X�V�C�x���g
	obj1.GetPhysicsEventManager().SendupdateAAABBEvent(obj1.GetObjID());
	obj2.GetPhysicsEventManager().SendupdateAAABBEvent(obj2.GetObjID());
}

void Rigidbody::StaticCollision(GameObject& obj, GameObject& other)
{
	ResolveOverlapStatic(obj, other);
	// ���݂̑��x���擾
	XMFLOAT3 v1 = obj.GetVelocity();
	v1.y = -v1.y * coefficient;
	// XMVECTOR�ɕϊ�
	XMVECTOR vec1 = XMLoadFloat3(&v1);

	/*   XMVECTOR normal = CalculateNormal(obj, other);

	   float velocityAlongNormal = CalculateDotProduct(vec1, normal);
	   XMVECTOR reflectedVelocity = vec1 - (1.0f + coefficient) * velocityAlongNormal * normal;*/
	   // �Փˌ�̑��x���v�Z
	   //XMVECTOR newVec1 = -vec1 * coefficient;
	   //XMVECTOR newVec2 = vec2 + ((2 * m1) / (m1 + m2)) * (vec1 - vec2) / (1 + coefficient);

	   // �v�Z���ʂ�ۑ�
	XMStoreFloat3(&v1, vec1);

	// �I�u�W�F�N�g�̑��x���X�V
	obj.SetVelocity(v1);
	//obj2.SetVelocity(v2);
}
void Rigidbody::DynamicCollision(GameObject& obj1, GameObject& obj2) {
	ResolveOverlapDynamic(obj1, obj2);

	// ���݂̑��x���擾
	XMFLOAT3 v1Float3 = obj1.GetVelocity();
	XMFLOAT3 v2Float3 = obj2.GetVelocity();

	XMFLOAT3 pos1Float3 = obj1.GetPos();
	XMFLOAT3 pos2Float3 = obj2.GetPos();

	XMFLOAT2 v1 = { v1Float3.x,v1Float3.y };
	XMFLOAT2 v2 = { v2Float3.x,v2Float3.y };

	XMFLOAT2 pos1 = { pos1Float3.x,pos1Float3.y };
	XMFLOAT2 pos2 = { pos2Float3.x,pos2Float3.y };

	// XMVECTOR�ɕϊ�
	XMVECTOR vec1 = XMLoadFloat2(&v1);
	XMVECTOR vec2 = XMLoadFloat2(&v2);
	XMVECTOR posVec1 = XMLoadFloat2(&pos1);
	XMVECTOR posVec2 = XMLoadFloat2(&pos2);

	// ���ʂ��v�Z
	XMFLOAT3 objSize = obj1.GetSize();
	XMFLOAT3 otherSize = obj2.GetSize();
	float m1 = objSize.x * objSize.y;
	float m2 = otherSize.x * otherSize.y;

	// �Փ˖@�����v�Z
	XMVECTOR normal = XMVector3Normalize(posVec2 - posVec1);

	// �e�I�u�W�F�N�g�̑��x��@�������ɕ���
	float v1n = XMVectorGetX(XMVector3Dot(vec1, normal)); // obj1�̖@���������x
	float v2n = XMVectorGetX(XMVector3Dot(vec2, normal)); // obj2�̖@���������x

	// �Փˌ�̖@���������x���v�Z�i�^���ʕۑ��� + �����W���j
	float v1n_new = (v1n * (m1 - 1 * m2) + (1 + 1) * m2 * v2n) / (m1 + m2);
	float v2n_new = (v2n * (m2 - 1 * m1) + (1 + 1) * m1 * v1n) / (m1 + m2);

	// �@�������̑��x�ω����v�Z
	XMVECTOR deltaV1 = (v1n_new - v1n) * normal;
	XMVECTOR deltaV2 = (v2n_new - v2n) * normal;

	// �Փˌ�̑��x���X�V
	vec1 += deltaV1;
	vec2 += deltaV2;

	// ���ʂ�ۑ�
	XMStoreFloat2(&v1, vec1);
	XMStoreFloat2(&v2, vec2);

	v1Float3 = { v1.x,v1.y,0 };
	v2Float3 = { v2.x,v2.y,0 };

	obj1.SetVelocity(v1Float3);
	obj2.SetVelocity(v2Float3);
	update_flg = true;
}

