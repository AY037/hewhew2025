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
GameObjectManager::GameObjectManager() {
	funcList["Background"] = []() { return std::make_shared<Background>(); };
	funcList["Player"] = []() { return std::make_shared<Player>(); };
	funcList["Enemy"] = []() { return std::make_shared<Enemy>(); };
	funcList["Stage"] = []() { return std::make_shared<Stage>(); };
	funcList["Ground"] = []() { return std::make_shared<Ground>(); };
	funcList["DragSword"] = []() { return std::make_shared<DragSword>(); };
	funcList["FlyEnemy"] = []() { return std::make_shared<FlyEnemy>(); };
	funcList["Bullet"] = []() { return std::make_shared<Bullet>(); };
	funcList["BulletFlyEnemy"] = []() { return std::make_shared<BulletFlyEnemy>(); };
}