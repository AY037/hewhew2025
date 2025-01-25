#include "Rigidbody.h"
#include "GameObject.h"
#include "Player.h"
#include <SimpleMath.h>
#include "Math.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
Rigidbody::Rigidbody()
{
}
void Rigidbody::Init(GameObject& obj)
{
	this->m_obj = &obj; // �A�h���X��ێ�����
	this->m_obj->GetPhysicsEventManager().AddonCollisionEvent(
		[this](GameObject& other,DirectX::XMFLOAT2& normal) {this->ResolveCollision(other, normal); });
}

void Rigidbody::Update()
{
	GameObject& obj = *static_cast<GameObject*>(this->m_obj);
	Vector3 velocity = obj.GetVelocity();

	XMVECTOR gravity_vec = XMLoadFloat3(&gravity);
	Vector3 airResi_vec = -velocity * airResistance;
    velocity += gravity_vec;
    velocity += airResi_vec;
    if (obj.GetName() == "Player")
    {
        if(velocity.y>0)
        {
            velocity += airResi_vec;
        }
    }


	obj.SetVelocity(velocity);
}

void Rigidbody::ResolveCollision(GameObject& other,DirectX::XMFLOAT2& normal) {

	GameObject& obj = *this->m_obj;

	if (other.GetObjectType() == true)
	{
		DynamicCollision(obj, other);
	}
	else
	{
        if (obj.GetName() == "Player")
        {
            Player* player = dynamic_cast<Player*>(&obj);
            if (player->GetPos().y > other.GetPos().y)
            {
                player->ResetJumpCnt();
            }
        }
		StaticCollision(obj, other, normal);
	}
}

void Rigidbody::StaticCollision(GameObject& obj, GameObject& other,DirectX::XMFLOAT2& normal)
{
	XMFLOAT3 pos1Float3 = obj.GetPos();
	XMFLOAT3 pos2Float3 = other.GetPos();

	XMFLOAT2 pos1 = { pos1Float3.x,pos1Float3.y };
	XMFLOAT2 pos2 = { pos1Float3.x,pos2Float3.y };

	XMVECTOR posVec1 = XMLoadFloat2(&pos1);
	XMVECTOR posVec2 = XMLoadFloat2(&pos2);
	// ���݂̑��x���擾
	XMFLOAT3 v1Float3 = obj.GetVelocity();
	XMFLOAT2 velocity = { v1Float3.x,v1Float3.y };

	// ���˃x�N�g�����v�Z����
	float radAngle =XMConvertToRadians(other.GetAngle());
	//XMFLOAT2 normal = { -std::sin(radAngle),std::cos(radAngle) };

	// other ���ÓI�I�u�W�F�N�g�ł��邽�߁A�@���x�N�g�����擾����
	XMVECTOR normalVec = XMLoadFloat2(&normal);
	XMVECTOR velocityVec = XMLoadFloat2(&velocity);

	// ���x��@�������ɕ������A���˃x�N�g�����v�Z
	XMVECTOR velocityDotNormal = XMVector2Dot(velocityVec, normalVec);
	XMVECTOR reflectionVec = velocityVec - (2.0f * velocityDotNormal * normalVec);

	// �����W�����|���Ĕ������x�𒲐�
	XMVECTOR finalVelocityVec = reflectionVec * coefficient;

	// �v�Z���ʂ�ۑ�
	XMStoreFloat2(&velocity, finalVelocityVec);

    //const float threshold = 0.05f;
    //if (fabs(velocity.x) < threshold && fabs(velocity.y) < threshold) {
    //    velocity.x = 0.0f;
    //    velocity.y = 0.0f;
    //}
    //if (fabs(velocity.x) < threshold && fabs(velocity.y) < threshold) {
    //    velocity.x = 0.0f;
    //    velocity.y = 0.0f;
    //}

	v1Float3 = { velocity.x,velocity.y,0 };
    if(obj.GetName()!="Player")
    {
        // ���I�I�u�W�F�N�g�̑��x���X�V
        obj.SetVelocity(v1Float3);
    }
}


void Rigidbody::DynamicCollision(GameObject& obj1, GameObject& obj2) {
    using namespace DirectX;

    // ���݂̑��x�ƈʒu���擾
    XMFLOAT3 v1Float3 = obj1.GetVelocity();
    XMFLOAT3 v2Float3 = obj2.GetVelocity();

    XMFLOAT3 pos1Float3 = obj1.GetPos();
    XMFLOAT3 pos2Float3 = obj2.GetPos();

    XMFLOAT2 v1 = { v1Float3.x, v1Float3.y };
    XMFLOAT2 v2 = { v2Float3.x, v2Float3.y };

    XMFLOAT2 pos1 = { pos1Float3.x, pos1Float3.y };
    XMFLOAT2 pos2 = { pos2Float3.x, pos2Float3.y };

    // XMVECTOR�ɕϊ�
    XMVECTOR vec1 = XMLoadFloat2(&v1);
    XMVECTOR vec2 = XMLoadFloat2(&v2);
    XMVECTOR posVec1 = XMLoadFloat2(&pos1);
    XMVECTOR posVec2 = XMLoadFloat2(&pos2);

    // ���ʂ��v�Z
    float m1 = obj1.GetMass();
    float m2 = obj2.GetMass();

    // �Փ˖@�����v�Z
    XMVECTOR normal = XMVector3Normalize(posVec2 - posVec1);

    // �ڐG�ʂ̐ڐ��������v�Z�i�@���ɐ����ȃx�N�g���j
    XMVECTOR tangent = XMVectorSet(-XMVectorGetY(normal), XMVectorGetX(normal), 0.0f, 0.0f);

    // �e�I�u�W�F�N�g�̑��x��@�������Ɛڐ������ɕ���
    float v1n = XMVectorGetX(XMVector3Dot(vec1, normal)); // obj1�̖@���������x
    float v2n = XMVectorGetX(XMVector3Dot(vec2, normal)); // obj2�̖@���������x

    float v1t = XMVectorGetX(XMVector3Dot(vec1, tangent)); // obj1�̐ڐ��������x
    float v2t = XMVectorGetX(XMVector3Dot(vec2, tangent)); // obj2�̐ڐ��������x

    // �Փˌ�̖@���������x���v�Z�i�^���ʕۑ��� + �����W���j
    float e = 1.1f; // �����W���i���S�e���Փ˂̏ꍇ1.0�j
    float v1n_new = (v1n * (m1 - e * m2) + (1 + e) * m2 * v2n) / (m1 + m2);
    float v2n_new = (v2n * (m2 - e * m1) + (1 + e) * m1 * v1n) / (m1 + m2);

    // �@�������̑��x�ω����v�Z
    XMVECTOR deltaV1_n = (v1n_new - v1n) * normal;
    XMVECTOR deltaV2_n = (v2n_new - v2n) * normal;

    // ���C���l�������ڐ������̑��x�ω�
    float frictionCoefficient = 0.3f; // ���C�W��
    float deltaV1_t = -frictionCoefficient * v1t;
    float deltaV2_t = -frictionCoefficient * v2t;

    XMVECTOR deltaV1_tangent = deltaV1_t * tangent;
    XMVECTOR deltaV2_tangent = deltaV2_t * tangent;

    // �Փˌ�̑��x���X�V
    vec1 += deltaV1_n + deltaV1_tangent;
    vec2 += deltaV2_n + deltaV2_tangent;

    // ���ʂ�ۑ�
    XMStoreFloat2(&v1, vec1);
    XMStoreFloat2(&v2, vec2);

    v1Float3 = { v1.x, v1.y, 0 };
    v2Float3 = { v2.x, v2.y, 0 };

    // ���x��臒l�ȉ��̏ꍇ�A�ړ����~
    const float threshold = 0.05f;
    if (fabs(v1Float3.x) < threshold && fabs(v1Float3.y) < threshold) {
        v1Float3.x = 0.0f;
        v1Float3.y = 0.0f;
    }
    if (fabs(v2Float3.x) < threshold && fabs(v2Float3.y) < threshold) {
        v2Float3.x = 0.0f;
        v2Float3.y = 0.0f;
    }

    if (obj2.GetName() == "Sword")
    {
        v1Float3.y = 2;
    }
    if(obj1.GetName()=="Sword")
    {
        v2Float3.y = 2;
    }

    obj1.SetVelocity(v1Float3);
    obj2.SetVelocity(v2Float3);
    update_flg = true;
}
