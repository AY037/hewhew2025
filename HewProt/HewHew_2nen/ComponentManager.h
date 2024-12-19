#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include "Component.h"

// ComponentManager
class ComponentManager
{
public:
    ComponentManager();

    ~ComponentManager() {}

    static ComponentManager& GetInstance()
    {
        static ComponentManager instance;
        return instance;
    }

    std::shared_ptr<Component> MakeComponent(const std::string& name) const
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
    std::unordered_map<std::string, std::function<std::shared_ptr<Component>()> > componentList;
};