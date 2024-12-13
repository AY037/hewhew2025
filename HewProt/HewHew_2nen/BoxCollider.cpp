#include "BoxCollider.h"
#include "GameObject.h"
#include "Rigidbody.h"
//using namespace DirectX;
void BoxCollider::Init(GameObject& obj)
{
    this->m_obj = &obj; // ƒAƒhƒŒƒX‚ð•ÛŽ‚·‚é
    this->m_obj->GetPhysicsEventManager().AddupdateAAABBEvent(
        [this](int objID) {this->tree.updateAABB(objID); });
    this->m_obj->SetIsBoxColl(true);
}
void BoxCollider::Update()
{
	HitCheck(*this->m_obj);
}
void BoxCollider::HitCheck(GameObject& obj)
{
	tree.findOverlappingObjects(obj.GetObjID());
}