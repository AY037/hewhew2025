#include "DynamicAABBTree.h"
#include "GameObject.h"
#include "Debug.h"
float Max(float i, float j)
{
	if (i >= j)return i;
	if (j > i)return j;
}
float Min(float i, float j)
{
	if (i >= j)return j;
	if (j > i)return i;
}

AABB DynamicAABBTree::GetAABB(std::shared_ptr<GameObject>& obj)
{
	int objID = obj->GetObjID();
	DirectX::XMFLOAT3 pos = obj->GetPos();
	DirectX::XMFLOAT3 size = obj->GetSize();
	AABB box;
	box.left = pos.x - size.x / 2;
	box.top = pos.y + size.y / 2;
	box.right = pos.x + size.x / 2;
	box.bottom = pos.y - size.y / 2;
	box.objctID = obj->GetObjID();

	return box;
}

void DynamicAABBTree::findOverlappingObjects(int _objectID) {
	const AABB& target = GetAABB((*objects)[_objectID]);

	int results = -1;
	query(root, target, results); // ルートから探索

	if (results != -1)
	{
		auto target = static_cast<GameObject*>((*objects)[results].get());
		if ((*objects)[_objectID]->GetObjectType() == true)
		{
			(*objects)[_objectID]->GetPhysicsEventManager().SendOnCollisionEvent(*target);
		}

	}
}

void DynamicAABBTree::updateDebugFlame()
{
	if (root == -1) {
		return;
	}

	// ルートから挿入位置を探す
	int size = nodes.size();
	// 近いオブジェクトどうしで親
	for (int i = 0;i<size; ++i) {
		if(nodes[i].left!=-1&& nodes[i].right != -1)
		{
			Debug::GetInstance().SetObjects(nodes[i].aabb,size);
		}
	}
}