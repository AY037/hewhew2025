#include "GameObjectManager.h"
#include "Background.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Ground.h"
#include "DragSword.h"
#include "FlyEnemy.h"
#include "Bullet.h"
#include "BulletFlyEnemy.h"
#include "FallGround.h"
#include "DontDragStage.h"
#include "FallWall.h"
#include "Coin.h"
#include "Goal.h"
#include "DynamicGround.h"

GameObjectManager::GameObjectManager() {
	funcList["Player"] = []() { return std::make_shared<Player>(); };
	funcList["Enemy"] = []() { return std::make_shared<Enemy>(); };
	funcList["Stage"] = []() { return std::make_shared<Stage>(); };
	funcList["Ground"] = []() { return std::make_shared<Ground>(); };
	funcList["DragSword"] = []() { return std::make_shared<DragSword>(); };
	funcList["FlyEnemy"] = []() { return std::make_shared<FlyEnemy>(); };
	funcList["Bullet"] = []() { return std::make_shared<Bullet>(); };
	funcList["BulletFlyEnemy"] = []() { return std::make_shared<BulletFlyEnemy>(); };
	funcList["FallGround"] = []() { return std::make_shared<FallGround>(); };
	funcList["DontDragStage"] = []() { return std::make_shared<DontDragStage>(); };
	funcList["FallWall"] = []() { return std::make_shared<FallWall>(); };
	funcList["Coin"] = []() { return std::make_shared<Coin>(); };
	funcList["Goal"] = []() { return std::make_shared<Goal>(); };
	funcList["DynamicGround"] = []() { return std::make_shared<DynamicGround>(); };
}