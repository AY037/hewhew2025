#include "GameObjectManager.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Ground.h"
GameObjectManager::GameObjectManager() {
	funcList["Background"] = []() { return std::make_shared<Background>(); };
	funcList["Player"] = []() { return std::make_shared<Player>(); };
	funcList["Enemy"] = []() { return std::make_shared<Enemy>(); };
	funcList["Stage"] = []() { return std::make_shared<Stage>(); };
	funcList["Ground"] = []() { return std::make_shared<Ground>(); };
	funcList["DragSword"] = []() { return std::make_shared<Ground>(); };
}