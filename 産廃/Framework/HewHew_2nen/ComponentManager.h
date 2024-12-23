#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "Transform.h"
#include "Rigidbody.h"
#include "BoxCollider.h"

// ComponentManager
template <class T>
class ComponentManager
{
public:
    ComponentManager() {
        // 初期登録するコンポーネント例
        //componentList["Transform"] = []() { return std::make_shared<Transform<T>>(); };
        componentList["BoxCollider"] = []() { return std::make_shared<BoxCollider>(); };
        componentList["Rigidbody"] = []() { return std::make_shared<Rigidbody>(); };
    }

    ~ComponentManager() {}

    static ComponentManager& GetInstance()
    {
        static ComponentManager instance;
        return instance;
    }

    std::shared_ptr<Component> GetComponent(const std::string& name) const
    {
        auto it = componentList.find(name);
        if (it != componentList.end()) {
            return it->second();
        }
        else {
            return nullptr;
        }
    }

private:
    // 型を std::shared_ptr<Component<T>> を返すラムダ式を格納する
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>()> > componentList;
};