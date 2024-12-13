#include "ComponentManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "BoxCollider.h"
#include "StageCollider.h"

ComponentManager::ComponentManager() {
    // 初期登録するコンポーネント例
    //componentList["Transform"] = []() { return std::make_shared<Transform<T>>(); };
    componentList["BoxCollider"] = []() { return std::make_shared<BoxCollider>(); };
    componentList["Rigidbody"] = []() { return std::make_shared<Rigidbody>(); };
    componentList["StageCollider"] = []() { return StageCollider::GetInstance(); };
}