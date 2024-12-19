#include "DynamicAABBTree.h"
#include "GameObject.h"
#include "Debug.h"
#include "Sound.h"
#include "EventManager.h"

//�Q�[���I�u�W�F�N�g�̃��X�g��n��
void DynamicAABBTree::Init(std::unordered_map<int, std::shared_ptr<GameObject>>& _objects)
{
	objects = &_objects;
}

// �m�[�h�̒ǉ�
int DynamicAABBTree::allocateNode() {
	if (freeNode >= nodes.size()) {
		nodes.push_back(AABBNode());
	}
	return freeNode++;
}

//�m�[�h�̏���
void DynamicAABBTree::deallocateNode(int nodeIndex) {
	nodes.erase(nodes.begin() + nodeIndex);
	freeNode--;
}

// AABB�����I�u�W�F�N�g���c���[�ɑ}��
void DynamicAABBTree::insertLeaf(int leaf) {
	if (root == -1) {
		root = leaf;
		nodes[root].parent = -1;
		return;
	}

	// ���[�g����}���ʒu��T��
	int current = root;
	// �߂��I�u�W�F�N�g�ǂ����Őe
	while (nodes[current].left != -1 && nodes[current].right != -1) {
		int left = nodes[current].left;
		int right = nodes[current].right;

		float leftCost = AABB::merge(nodes[left].aabb, nodes[leaf].aabb).surfaceArea();
		float rightCost = AABB::merge(nodes[right].aabb, nodes[leaf].aabb).surfaceArea();

		// �œK�ȕ����ɐi��
		if (leftCost < rightCost) {
			current = left;
		}
		else {
			current = right;
		}
	}

	// �V�����e�m�[�h���쐬
	int oldParent = nodes[current].parent;
	int newParent = allocateNode();

	nodes[newParent].parent = oldParent;
	nodes[newParent].aabb = AABB::merge(nodes[current].aabb, nodes[leaf].aabb);
	nodes[newParent].left = current;
	nodes[newParent].right = leaf;
	nodes[current].parent = newParent;
	nodes[leaf].parent = newParent;


	if (oldParent == -1) {
		root = newParent;
	}
	else {
		if (nodes[oldParent].left == current) {
			nodes[oldParent].left = newParent;
		}
		else {
			nodes[oldParent].right = newParent;
		}
	}

	// �������X�V
	updateHeight(newParent);
}

//�q�m�[�h��AABB���X�V�����Ɛe�m�[�h��AABB���X�V
void DynamicAABBTree::UpdateParentAABB(int node) {
	while (node != -1) {
		int left = nodes[node].left;
		int right = nodes[node].right;

		// �q�m�[�h���}�[�W���Č��݂̃m�[�h��AABB���X�V
		nodes[node].aabb = AABB::merge(nodes[left].aabb, nodes[right].aabb);

		// �e�m�[�h�ɐi��
		node = nodes[node].parent;
	}
}

// �������X�V
void DynamicAABBTree::updateHeight(int nodeIndex) {
	//while (nodeIndex != -1) {
	//	nodes[nodeIndex].height = 1 + std::max(
	//		nodes[nodes[nodeIndex].left].height,
	//		nodes[nodes[nodeIndex].right].height
	//	);
	//	nodeIndex = nodes[nodeIndex].parent;
	//}
}

AABB DynamicAABBTree::GetAABB(std::shared_ptr<GameObject>& obj)
{
	float angle = obj->GetAngle();
	if (angle != 0)
	{
		DirectX::XMFLOAT3 pos = obj->GetPos();
		DirectX::XMFLOAT3 size = obj->GetSize();
		float radAngle = DirectX::XMConvertToRadians(angle);

		// AABB�̃R�[�i�[���v�Z
		DirectX::XMFLOAT2 corners[4] = {
			{ -size.x / 2, size.y / 2},
			{ size.x / 2, size.y / 2},
			{ -size.x / 2,  -size.y / 2},
			{ size.x / 2,  -size.y / 2 }
		};

		// ��]�s����쐬
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationZ(radAngle);

		// ��]��̃R�[�i�[���v�Z
		DirectX::XMFLOAT2 rotatedCorners[4];
		for (int i = 0; i < 4; ++i) {
			DirectX::XMVECTOR corner = XMLoadFloat2(&corners[i]);
			corner = XMVector2Transform(corner, rotationMatrix);
			XMStoreFloat2(&rotatedCorners[i], corner);
			//��]�㌳�̍��W�ֈړ�
			rotatedCorners[i].x += pos.x;
			rotatedCorners[i].y += pos.y;
		}
		//��]���ꂽ�I�u�W�F�N�g��AABB�����߂�
		AABB box = { rotatedCorners[0].x,rotatedCorners[0].y,rotatedCorners[0].x,rotatedCorners[0].y };
		for (int i = 1; i < 4; ++i) {
			if (box.left > rotatedCorners[i].x) box.left = rotatedCorners[i].x;
			if (box.top < rotatedCorners[i].y) box.top = rotatedCorners[i].y;
			if (box.right < rotatedCorners[i].x) box.right = rotatedCorners[i].x;
			if (box.bottom > rotatedCorners[i].y) box.bottom = rotatedCorners[i].y;
		}
		box.objectID = obj->GetObjID();
		return box;
	}
	else
	{
		DirectX::XMFLOAT3 pos = obj->GetPos();
		DirectX::XMFLOAT3 size = obj->GetSize();
		AABB box;
		box.left = pos.x - size.x / 2;
		box.top = pos.y + size.y / 2;
		box.right = pos.x + size.x / 2;
		box.bottom = pos.y - size.y / 2;
		box.objectID = obj->GetObjID();

		return box;
	}
}

bool DynamicAABBTree::findOverlappingObjects(int _objectID) {
	const AABB& target = GetAABB((*objects)[_objectID]);

	std::unordered_map<int, DirectX::XMFLOAT2> results;
	query(root, target, results); // ���[�g����T��

	if (results.size() != 0)
	{
		for (auto& _result : results)
		{
			auto target = static_cast<GameObject*>((*objects)[_result.first].get());
			(*objects)[_objectID]->GetPhysicsEventManager().SendOnCollisionEvent(*target, _result.second);

		}
	}

	if (results.size() > 0)
	{
		return true;
	}
}

// �I�u�W�F�N�g��}��
int DynamicAABBTree::insert(const AABB& aabb, int objectID) {
	int nodeIndex = allocateNode();
	nodes[nodeIndex].aabb = aabb;
	nodes[nodeIndex].objectID = objectID;
	insertLeaf(nodeIndex);
	objectToNodeMap[objectID] = nodeIndex;//�I�u�W�F�N�gID�ƃm�[�h�̕R�Â�
	return nodeIndex;
}

void DynamicAABBTree::SetLeaf()
{
	nodes.resize(objects->size());
	for (auto& iter : *objects)
	{
		if (iter.second->GetIsBoxColl() == true)
		{
			auto& obj = iter.second;
			AABB aabb = GetAABB(obj);
			int id2 = insert(aabb, iter.first);
		}
	}
}

void DynamicAABBTree::Update() {
	remove();         // �Â��m�[�h���c���[����폜
	SetLeaf();
}

//�d�Ȃ��Ă���I�u�W�F�N�g�̒T��
void DynamicAABBTree::query(int nodeID, const AABB& target, std::unordered_map<int, DirectX::XMFLOAT2>& results) {
	if (nodeID == -1) return; // �����ȃm�[�h�͖���

	for (const AABBNode& node : nodes)
	{
		// AABB���d�Ȃ��Ă���ꍇ�̂ݏ���
		if (node.aabb.intersects(target))
		{
			if (node.isLeaf()) {
				int objID1 = node.objectID;
				int objID2 = target.objectID;
				std::shared_ptr<GameObject>& obj1 = (*objects)[objID1];
				std::shared_ptr<GameObject>& obj2 = (*objects)[objID2];
				DirectX::XMFLOAT2 normal;

				//Player��Ground�ȊO����
				if (obj1->GetName() == "Player" || obj2->GetName() == "Player")
				{
					if (obj1->GetName() == "Ground" || obj2->GetName() == "Ground")
					{
						if (obj1->GetObjectType() == true)
						{
							//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
							if (obb.IntersectsWithNormal(obj1, obj2, normal))
							{
								// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
								results[target.objectID] = normal;
							}
						}
						if (obj2->GetObjectType() == true)
						{
							//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
							if (obb.IntersectsWithNormal(obj2, obj1, normal))
							{
								// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
								results[node.objectID] = normal;
							}
						}
					}
					if (obj1->GetName() == "Enemy" || obj2->GetName() == "Enemy")
					{
						if (obj1->GetObjectType() == true)
						{
							EventManager::GetInstance().SendEvent("damage");
						}
					}
				}
				else
				{
					if (obj1->GetName() == "DragSword" || obj2->GetName() == "DragSword")
					{
						if (obj1->GetName() == "Debri" || obj2->GetName() == "Debri")
						{
							if (obj1->GetObjectType() == true)
							{
								//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
								if (obb.IntersectsWithNormal((*objects)[objID1], (*objects)[objID2], normal))
								{
									// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
									results[node.objectID] = normal;
								}
							}
							if (obj2->GetObjectType() == true)
							{
								//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
								if (obb.IntersectsWithNormal((*objects)[objID2], (*objects)[objID1], normal))
								{
									// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
									results[target.objectID] = normal;
								}
							}
						}
					}
					else
					{

						if (obj1->GetObjectType() == true)
						{
							//�c�[�̈ړ����x���������
							if (obj2->GetName() == "Debri")
							{
								if (math::Max(std::fabs(obj2->GetVelocity().x), std::fabs(obj2->GetVelocity().y)) > 1.0f)
								{
									Sound::GetInstance().Play(SE_DESTROY);
								}
							}
							//�c�[�̈ړ����x���������
							if (obj1->GetName() == "Debri")
							{
								if (math::Max(std::fabs(obj1->GetVelocity().x), std::fabs(obj1->GetVelocity().y)) > 1.0f)
								{
									Sound::GetInstance().Play(SE_DESTROY);
								}
							}

							//�U�����G�ɖ��������Ƃ�
							if (obj2->GetName() == "Sword" && obj1->GetName() == "Enemy")
							{
								obj1->SetSize(5.0f, 5.0f, 0);
								obj1->SetName("Debri");
								Sound::GetInstance().Play(SE_HIT);
							}
							if (obj1->GetName() == "Sword" && obj2->GetName() == "Enemy")
							{
								obj2->SetSize(5.0f, 5.0f, 0);
								obj2->SetName("Debri");
								Sound::GetInstance().Play(SE_HIT);
							}
						}
						if (obj2->GetName() == "Debri" && obj1->GetName() == "Debri")
						{
							//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
							if (obb.IntersectsWithNormal((*objects)[objID1], (*objects)[objID2], normal))
							{
								// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
								results[node.objectID] = normal;
							}
						}
						else
						{
							//OBB�ŏd�Ȃ��Ă邩�`�F�b�N
							if (obb.IntersectsWithNormal((*objects)[objID1], (*objects)[objID2], normal))
							{
								// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
								results[node.objectID] = normal;
							}
						}
					}
				}
			}
		}
	}
}

//�m�[�h�S�̂�AABB�̍X�V
void DynamicAABBTree::updateAABB(int objID)
{
	auto& obj = (*objects)[objID];
	nodes[objectToNodeMap[objID]].aabb = GetAABB(obj);
	int size = nodes.size();
	for (int i = 0; i < size; ++i)
	{
		if (nodes[i].left != -1 && nodes[i].right != -1)
		{
			UpdateParentAABB(i);
		}
	}
}

// �I�u�W�F�N�g���폜
void DynamicAABBTree::remove() {
	nodes.clear();
	objectToNodeMap.clear();
	root = -1;
	freeNode = 0;
}

void DynamicAABBTree::updateDebugFlame()
{
	if (root == -1) {
		return;
	}

	// ���[�g����}���ʒu��T��
	int size = nodes.size();
	// �߂��I�u�W�F�N�g�ǂ����Őe
	for (int i = 0; i < size; ++i) {
		if (nodes[i].left != -1 && nodes[i].right != -1)
		{
			Debug::GetInstance().SetObjects(nodes[i].aabb, size);
		}
	}
}