#pragma once
#include <functional>
#include <unordered_map>
#include <string>
class GameObject;
class PhysicsEventManager
{
public:
	PhysicsEventManager() {}
	~PhysicsEventManager() {}
	void AddonCollisionEvent(std::function<void(GameObject&)> _func)
	{
		onCollisionFunc = _func;
		rigidbody_Flg = true;
	}

	void SendOnCollisionEvent(GameObject& other)
	{
		if(onCollisionFunc)
		onCollisionFunc(other);
	}

	void AddupdateAAABBEvent(std::function<void(int)> _func)
	{
		updateAAABB = _func;
	}

	void SendupdateAAABBEvent(int objID)
	{
		updateAAABB(objID);
	}

private:
	std::function<void(GameObject&)> onCollisionFunc;
	std::function<void(int)> updateAAABB;
	bool rigidbody_Flg = false;
};