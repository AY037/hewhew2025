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
	//�����蔻��S�I�u�W�F�N�g�Ƃ̔�r�p
	bool HitCheck(GameObject& obj);
	std::vector<std::shared_ptr<GameObject>> HitObjectName(GameObject& obj);
	//�����蔻�����I�u�W�F�N�g�Ƃ̔�r�p
	bool HitCheck(GameObject& obj, std::string target, int* enemyId = nullptr);
	//�����蔻�����I�u�W�F�N�gID�Ƃ̔�r�p
	bool HitCheck(GameObject& obj,int targetId);

	void Update()override;

private:
	DynamicAABBTree& tree = DynamicAABBTree::GetInstance();
};