#pragma once
#include "DynamicAABBTree.h"
#include "Component.h"
#include <string>

class BoxCollider :public Component
{
public:
	BoxCollider() {
	}
	~BoxCollider() {}
	void Init(GameObject& obj)override;
	//当たり判定全オブジェクトとの比較用
	bool HitCheck(GameObject& obj);
	std::vector<std::shared_ptr<GameObject>> HitObjectName(GameObject& obj);
	//当たり判定特定オブジェクトとの比較用
	bool HitCheck(GameObject& obj, std::string target, int* enemyId = nullptr);
	//当たり判定特定オブジェクトIDとの比較用
	bool HitCheck(GameObject& obj,int targetId);

	void Update()override;

private:
	DynamicAABBTree& tree = DynamicAABBTree::GetInstance();
};