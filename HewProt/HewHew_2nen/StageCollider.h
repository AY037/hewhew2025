#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "ColliderStructure.h"

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
	bool HitCheck(GameObject& obj);
	void Update()override;
	void SetGhost();

	void reset() {
		pGhostFlg = false;
		stageObjects.clear();
	}
private:
	std::vector<GameObject*> stageObjects;
	OBB obb;
	bool pGhostFlg = false;
};

