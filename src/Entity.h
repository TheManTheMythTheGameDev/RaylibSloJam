#pragma once
#include "component.h"
#include <vector>

class Entity
{
public:
    Entity();
    ~Entity()
    {
        for (component* comp : components)
        {
            delete comp;
        }
    }

    template<typename T, typename... Args>
    inline void AddComponent(Args... constructorArgs)
    {
        components.push_back((component*)(new T(constructorArgs)));
    }

    inline void AddComponent(component* comp)
    {
        components.push_back(comp);
    }
private:
    std::vector<component*> components;
};
