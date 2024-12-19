#pragma once
#include "ColliderStructure.h"
#include <unordered_map>

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
	void Init(std::unordered_map<int, std::shared_ptr<GameObject>>& _objects);

	// ノードの追加
	int allocateNode();

	//ノードの消去
	void deallocateNode(int nodeIndex);

	// AABBを持つオブジェクトをツリーに挿入
	void insertLeaf(int leaf);

	//子ノードのAABBが更新されると親ノードのAABBも更新
	void UpdateParentAABB(int node);

	// 高さを更新
	void updateHeight(int nodeIndex);

	// オブジェクトを挿入
	int insert(const AABB& aabb, int objectID);

	//オブジェクトで葉を作る
	void SetLeaf();

	//ツリーの更新
	void Update();

	//重なっているオブジェクトの探索
	void query(int nodeID, const AABB& target, std::unordered_map<int, DirectX::XMFLOAT2>& results);

	//ノード全体のAABBの更新
	void updateAABB(int objID);

	//重なっているオブジェクトを探す
	bool findOverlappingObjects(int _objectID);

	// オブジェクトを削除
	void remove();

	//AABBフレーム描画用
	void updateDebugFlame();

private:
	std::vector<AABBNode> nodes; // ノードリスト
	int root = -1; // ルートノードのインデックス
	int freeNode = 0; // 次に使用可能なノードインデックス
	OBB obb;

	std::unordered_map<int, std::shared_ptr<GameObject>>* objects = nullptr;
	std::unordered_map<int, int> objectToNodeMap;

};