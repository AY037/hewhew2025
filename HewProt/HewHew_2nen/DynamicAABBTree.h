#pragma once
#include "ColliderStructure.h"
#include <unordered_map>
#include <string>

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
public:
	DynamicAABBTree() {}
	~DynamicAABBTree() {}


	static DynamicAABBTree& GetInstance() {
		static DynamicAABBTree instance;
		return instance;
	}

	//�I�u�W�F�N�g�̍��W�ƃT�C�Y����AABB���v�Z
	AABB GetAABB(std::shared_ptr<GameObject>& obj);

	//�Q�[���I�u�W�F�N�g�̃��X�g��n��
	void Init(std::unordered_map<int, std::shared_ptr<GameObject>>& _objects);

	// �m�[�h�̒ǉ�
	int allocateNode();

	//�m�[�h�̏���
	void deallocateNode(int nodeIndex);

	void updateAABB(int node);
	void updateTree();
	// AABB�����I�u�W�F�N�g���c���[�ɑ}��
	void insertLeaf(int leaf);

	//�q�m�[�h��AABB���X�V�����Ɛe�m�[�h��AABB���X�V
	void UpdateParentAABB(int node);

	// �������X�V
	void updateHeight(int nodeIndex);

	// �I�u�W�F�N�g��}��
	int insert(const AABB& aabb, int objectID);

	//�I�u�W�F�N�g�ŗt�����
	void SetLeaf();

	//�c���[�̍X�V
	void Update();

	//�d�Ȃ��Ă���I�u�W�F�N�g�̒T��
	void query(int nodeID, const AABB& target, std::unordered_map<int, DirectX::XMFLOAT2>& results);

	//����̏d�Ȃ��Ă���I�u�W�F�N�g�̒T��
	void query(int nodeID, const AABB& target, std::unordered_map<int, DirectX::XMFLOAT2>& results, std::string& targetName);;

	//�d�Ȃ��Ă���I�u�W�F�N�g��T��
	bool findOverlappingObjects(int _objectID);

	//����̏d�Ȃ��Ă���I�u�W�F�N�g��T��
	bool findOverlappingObjects(int _objectID, std::string& targetName, int* enemyId =nullptr);

	// �I�u�W�F�N�g���폜
	void reset();

	//AABB�t���[���`��p
	void updateDebugFlame();

private:
	std::vector<AABBNode> nodes; // �m�[�h���X�g
	int root = -1; // ���[�g�m�[�h�̃C���f�b�N�X
	int freeNode = 0; // ���Ɏg�p�\�ȃm�[�h�C���f�b�N�X
	OBB obb;

	std::unordered_map<int, std::shared_ptr<GameObject>>* objects = nullptr;
	std::unordered_map<int, int> objectToNodeMap;

};