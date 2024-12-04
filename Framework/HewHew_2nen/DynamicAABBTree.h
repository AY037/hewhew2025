#pragma once
#include "Component.h"
#include <unordered_map>
#include <memory>
#include <iostream>

class GameObject;

float Max(float i, float j);
float Min(float i, float j);

struct AABB
{
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int objctID = -1;
	// �I�u�W�F�N�g���m�̌�������
	bool intersects(const AABB& other) const {
		if (objctID != other.objctID)
		{
			if ((right >= other.left && left <= other.left) || (right >= other.right && left <= other.right))
			{
				if ((bottom <= other.top && top >= other.top) || (bottom <= other.bottom && top >= other.bottom))
				{
					return true;
				}
			}
		}
		return false;
	}

	// AABB�𓝍�����֐�
	static AABB merge(const AABB& a, const AABB& b) {
		AABB aabb;
		aabb.left = Min(a.left, b.left);
		aabb.top = Max(a.top, b.top);
		aabb.right = Max(a.right, b.right);
		aabb.bottom = Min(a.bottom, b.bottom);
		return aabb;
	}

	// AABB�̕\�ʐρi�R�X�g�v�Z�Ŏg�p�j
	float surfaceArea() const {
		float dx = right - left;
		float dy = top - bottom;
		return 2.0f * (dx * dy);
	}

	bool contains(const AABB& other)
	{
		if ((left == other.left && top == other.top) && (right == other.right && top == other.top))
		{
			return true;
		}

		return false;
	}
};

struct AABBNode {
	AABB aabb; // �m�[�h���Ǘ�����AABB
	int parent = -1; // �e�m�[�h�̃C���f�b�N�X
	int left = -1;   // ���̎q�m�[�h�̃C���f�b�N�X
	int right = -1;  // �E�̎q�m�[�h�̃C���f�b�N�X
	int height = 0;  // �c���[�̍���
	int objectID = -1; // �i�[����I�u�W�F�N�g��ID (-1�̓��[�t�łȂ����Ƃ�����)

	bool isLeaf() const {
		return left == -1 && right == -1;
	}
};
class DynamicAABBTree {
private:
	std::vector<AABBNode> nodes; // �m�[�h���X�g
	int root = -1; // ���[�g�m�[�h�̃C���f�b�N�X
	int freeNode = 0; // ���Ɏg�p�\�ȃm�[�h�C���f�b�N�X

	std::unordered_map<int, std::shared_ptr<GameObject>>* objects = nullptr;
	std::unordered_map<int, int> objectToNodeMap;
	// �m�[�h�̒ǉ�
	int allocateNode() {
		if (freeNode >= nodes.size()) {
			nodes.push_back(AABBNode());
		}
		return freeNode++;
	}

	void deallocateNode(int nodeIndex) {
		nodes.erase(nodes.begin() + nodeIndex);
		freeNode--;
	}

	// AABB�����I�u�W�F�N�g���c���[�ɑ}��
	void insertLeaf(int leaf) {
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
		//UpdateParentAABB(newParent);


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
	void UpdateParentAABB(int node) {
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
	void updateHeight(int nodeIndex) {
		//while (nodeIndex != -1) {
		//	nodes[nodeIndex].height = 1 + std::max(
		//		nodes[nodes[nodeIndex].left].height,
		//		nodes[nodes[nodeIndex].right].height
		//	);
		//	nodeIndex = nodes[nodeIndex].parent;
		//}
	}

public:
	DynamicAABBTree() {}
	~DynamicAABBTree() {}


	static DynamicAABBTree& GetInstance() {
		static DynamicAABBTree instance;
		return instance;
	}

	AABB GetAABB(std::shared_ptr<GameObject>& obj);

	void Init(std::unordered_map<int, std::shared_ptr<GameObject>>& _objects)
	{
		objects = &_objects;
	}

	// �I�u�W�F�N�g��}��
	int insert(const AABB& aabb, int objectID) {
		int nodeIndex = allocateNode();
		nodes[nodeIndex].aabb = aabb;
		nodes[nodeIndex].objectID = objectID;
		insertLeaf(nodeIndex);
		objectToNodeMap[objectID] = nodeIndex;//�I�u�W�F�N�gID�ƃm�[�h�̕R�Â�
		return nodeIndex;
	}

	void SetLeaf()
	{
		for (auto& iter : *objects)
		{
			auto& obj = iter.second;
			AABB aabb = GetAABB(obj);
			int id2 = insert(aabb, iter.first);
		}
	}

	void Update() {
		for (auto& iter : *objects)
		{
			int objectID = iter.first;
			AABB newAABB = GetAABB(iter.second);
			int nodeIndex = objectToNodeMap[objectID]; // �I�u�W�F�N�gID�ɑΉ�����m�[�h���擾
			if (nodeIndex == -1) return;       // �m�[�h��������Ȃ���Ή������Ȃ�

			// �Â�AABB�ƐV����AABB���قȂ�ꍇ
			if (!nodes[nodeIndex].aabb.contains(newAABB)) {
				int ID = nodes[nodeIndex].objectID;
				remove(nodeIndex);         // �Â��m�[�h���c���[����폜
				SetLeaf();
			}
		}
	}


	void query(int nodeID, const AABB& target, int& results) {
		if (nodeID == -1) return; // �����ȃm�[�h�͖���

		const AABBNode& node = nodes[nodeID];


		// AABB���d�Ȃ��Ă���ꍇ�̂ݏ���
		if (node.aabb.intersects(target)) {
			if (node.isLeaf()) {
				// ���[�t�m�[�h�Ȃ�A�I�u�W�F�N�gID�����ʂɒǉ�
				results = node.objectID;
			}
			else {
				// �q�m�[�h���ċA�I�ɒT��
				query(node.left, target, results);
				query(node.right, target, results);
			}
		}
	}

	void updateAABB(int objID)
	{
		auto& obj = (*objects)[objID];
		nodes[objectToNodeMap[objID]].aabb = GetAABB(obj);
		int size = nodes.size();
		for(int i=0;i< size;++i)
		{
			if(nodes[i].left != -1 && nodes[i].right != -1)
			{
				UpdateParentAABB(i);
			}
		}
	}

	//�ÓI�I�u�W�F�N�g�p
	void findOverlappingObjects(int _objectID);

	void updateDebugFlame();
	// �I�u�W�F�N�g���폜
	void remove(int nodeIndex) {
		nodes.clear();
		objectToNodeMap.clear();
		root = -1;
		freeNode = 0;
	}
};

