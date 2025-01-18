#include "BoxCollider.h"
#include "GameObject.h"
#include "Rigidbody.h"
//using namespace DirectX;
void BoxCollider::Init(GameObject& obj)
{
    this->m_obj = &obj; // �A�h���X��ێ�����
    this->m_obj->SetIsBoxColl(true);
}

void BoxCollider::Update()
{
	HitCheck(*this->m_obj);
}

//�����蔻��S�I�u�W�F�N�g�Ƃ̔�r�p
bool BoxCollider::HitCheck(GameObject& obj)
{
	return tree.findOverlappingObjects(obj.GetObjID());
}

//�����蔻��S�I�u�W�F�N�g�Ƃ̔�r�p
std::unordered_map<std::string, std::shared_ptr<GameObject>> BoxCollider::HitObjectName(GameObject& obj)
{
	return tree.findOverlappingObjectName(obj.GetObjID());
}

//�����蔻�����I�u�W�F�N�g�Ƃ̔�r�p
bool BoxCollider::HitCheck(GameObject& obj, std::string targetName, int* enemyId)
{
    return tree.findOverlappingObjects(obj.GetObjID(), targetName, enemyId);
}