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
	// オブジェクト同士の交差判定
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

	// AABBを統合する関数
	static AABB merge(const AABB& a, const AABB& b) {
		AABB aabb;
		aabb.left = Min(a.left, b.left);
		aabb.top = Max(a.top, b.top);
		aabb.right = Max(a.right, b.right);
		aabb.bottom = Min(a.bottom, b.bottom);
		return aabb;
	}

	// AABBの表面積（コスト計算で使用）
	float surfaceArea() const {
		float dx = right - left;
		float dy = top - bottom;
		return 2.0f * (dx * dy);
	}

	//AABBが更新されているか確認
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
	AABB aabb; // ノードが管理するAABB
	int parent = -1; // 親ノードのインデックス
	int left = -1;   // 左の子ノードのインデックス
	int right = -1;  // 右の子ノードのインデックス
	int height = 0;  // ツリーの高さ
	int objectID = -1; // 格納するオブジェクトのID (-1はリーフでないことを示す)

	bool isLeaf() const {
		return left == -1 && right == -1;
	}
};

class DynamicAABBTree {
private:
	std::vector<AABBNode> nodes; // ノードリスト
	int root = -1; // ルートノードのインデックス
	int freeNode = 0; // 次に使用可能なノードインデックス

	std::unordered_map<int, std::shared_ptr<GameObject>>* objects = nullptr;
	std::unordered_map<int, int> objectToNodeMap;
	// ノードの追加
	int allocateNode() {
		if (freeNode >= nodes.size()) {
			nodes.push_back(AABBNode());
		}
		return freeNode++;
	}

	//ノードの消去
	void deallocateNode(int nodeIndex) {
		nodes.erase(nodes.begin() + nodeIndex);
		freeNode--;
	}

	// AABBを持つオブジェクトをツリーに挿入
	void insertLeaf(int leaf) {
		if (root == -1) {
			root = leaf;
			nodes[root].parent = -1;
			return;
		}

		// ルートから挿入位置を探す
		int current = root;
		// 近いオブジェクトどうしで親
		while (nodes[current].left != -1 && nodes[current].right != -1) {
			int left = nodes[current].left;
			int right = nodes[current].right;

			float leftCost = AABB::merge(nodes[left].aabb, nodes[leaf].aabb).surfaceArea();
			float rightCost = AABB::merge(nodes[right].aabb, nodes[leaf].aabb).surfaceArea();

			// 最適な方向に進む
			if (leftCost < rightCost) {
				current = left;
			}
			else {
				current = right;
			}
		}

		// 新しい親ノードを作成
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

		// 高さを更新
		updateHeight(newParent);
	}
	//子ノードのAABBが更新されると親ノードのAABBも更新
	void UpdateParentAABB(int node) {
		while (node != -1) {
			int left = nodes[node].left;
			int right = nodes[node].right;

			// 子ノードをマージして現在のノードのAABBを更新
			nodes[node].aabb = AABB::merge(nodes[left].aabb, nodes[right].aabb);

			// 親ノードに進む
			node = nodes[node].parent;
		}
	}

	// 高さを更新
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

	//オブジェクトの座標とサイズからAABBを計算
	AABB GetAABB(std::shared_ptr<GameObject>& obj);

	//ゲームオブジェクトのリストを渡す
	void Init(std::unordered_map<int, std::shared_ptr<GameObject>>& _objects)
	{
		objects = &_objects;
	}

	// オブジェクトを挿入
	int insert(const AABB& aabb, int objectID) {
		int nodeIndex = allocateNode();
		nodes[nodeIndex].aabb = aabb;
		nodes[nodeIndex].objectID = objectID;
		insertLeaf(nodeIndex);
		objectToNodeMap[objectID] = nodeIndex;//オブジェクトIDとノードの紐づけ
		return nodeIndex;
	}

	//オブジェクトで葉を作る
	void SetLeaf()
	{
		for (auto& iter : *objects)
		{
			auto& obj = iter.second;
			AABB aabb = GetAABB(obj);
			int id2 = insert(aabb, iter.first);
		}
	}

	//ツリーの更新
	void Update() {
		for (auto& iter : *objects)
		{
			if(&iter!=nullptr)
			{
				int objectID = iter.first;
				AABB newAABB = GetAABB(iter.second);
				int nodeIndex = objectToNodeMap[objectID]; // オブジェクトIDに対応するノードを取得
				if (nodeIndex == -1) return;       // ノードが見つからなければ何もしない

				// 古いAABBと新しいAABBが異なる場合
				if (!nodes[nodeIndex].aabb.contains(newAABB)) {
					int ID = nodes[nodeIndex].objectID;
					remove(nodeIndex);         // 古いノードをツリーから削除
					SetLeaf();
				}
			}
		}
	}

	//重なっているオブジェクトの探索
	void query(int nodeID, const AABB& target, int& results) {
		if (nodeID == -1) return; // 無効なノードは無視

		const AABBNode& node = nodes[nodeID];


		// AABBが重なっている場合のみ処理
		if (node.aabb.intersects(target)) {
			if (node.isLeaf()) {
				// リーフノードなら、オブジェクトIDを結果に追加
				results = node.objectID;
			}
			else {
				// 子ノードを再帰的に探索
				query(node.left, target, results);
				query(node.right, target, results);
			}
		}
	}

	//ノード全体のAABBの更新
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

	//重なっているオブジェクトを探す
	void findOverlappingObjects(int _objectID);

	//AABBフレーム描画用
	void updateDebugFlame();
	// オブジェクトを削除
	void remove(int nodeIndex) {
		nodes.clear();
		objectToNodeMap.clear();
		root = -1;
		freeNode = 0;
	}
};

