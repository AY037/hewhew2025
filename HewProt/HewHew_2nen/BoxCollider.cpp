#include "BoxCollider.h"
#include "GameObject.h"
#include "Rigidbody.h"
//using namespace DirectX;
void BoxCollider::Init(GameObject& obj)
{
    this->m_obj = &obj; // アドレスを保持する
    this->m_obj->SetIsBoxColl(true);
}

void BoxCollider::Update()
{
	HitCheck(*this->m_obj);
}

//当たり判定全オブジェクトとの比較用
bool BoxCollider::HitCheck(GameObject& obj)
{
	return tree.findOverlappingObjects(obj.GetObjID());
}

//当たり判定全オブジェクトとの比較用
std::vector<std::shared_ptr<GameObject>> BoxCollider::HitObjectName(GameObject& obj)
{
	return tree.findOverlappingObjectName(obj.GetObjID());
}

//当たり判定特定オブジェクトとの比較用
bool BoxCollider::HitCheck(GameObject& obj, std::string targetName, int* enemyId)
{
    return tree.findOverlappingObjects(obj.GetObjID(), targetName, enemyId);
}

//当たり判定特定オブジェクトIDとの比較用
bool BoxCollider::HitCheck(GameObject& obj, int targetId)
{
    return tree.checkHit(obj.GetObjID(), targetId);
}