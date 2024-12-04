#pragma once
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include <memory>
#include <functional>
class GameObjectManager
{
public:
	GameObjectManager() {
		funcList["Background"] = []() { return std::make_shared<Background>(); };
		funcList["Player"] = []() { return std::make_shared<Player>(); };
		funcList["Enemy"] = []() { return std::make_shared<Enemy>(); };
	}
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

private:
	std::unordered_map<std::string, std::function<std::shared_ptr<GameObject>()>> funcList;
	//                 ->first()     ->second()
};