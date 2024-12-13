#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <iostream>
#include "GameObject.h"
class GameObjectManager
{
public:
	GameObjectManager();
	static GameObjectManager& GetInstance() {
		static GameObjectManager gameObjectManager;
		return gameObjectManager;
	}
	std::shared_ptr<GameObject> GetObj(std::string _name)
	{
		auto it = funcList.find(_name);
		if (it != funcList.end()) {
			return it->second();
		}
		else
		{
			std::cout << "Error in GameObjectManager" << std::endl;
		}
	}
	std::unordered_map<std::string, std::function<std::shared_ptr<GameObject>()>>& GetMakeObjectList()
	{
		return funcList;
	}
private:
	std::unordered_map<std::string, std::function<std::shared_ptr<GameObject>()>> funcList;
	//                 ->first()     ->second()
};