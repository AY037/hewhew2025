#pragma once
#include "Component.h"
#include <vector>
#include <memory>

class StageCollider:public Component
{
public:
	StageCollider() { stageObjects.resize(0); }
	~StageCollider() {}
	static std::shared_ptr<StageCollider>& GetInstance()
	{
		static std::shared_ptr<StageCollider> instance = std::make_shared<StageCollider>();
		return instance;
	}
	void Init(GameObject& obj)override;
	void HitCheck(GameObject& obj);
	void Update()override;

	void SetGhost();
private:
	std::vector<GameObject*> stageObjects;
	bool pGhostFlg = false;
};

