#pragma once
#include "component.h"
#include <vector>

class Entity
{
public:
    Entity()
    {
    }
    ~Entity()
    {
        for (component* comp : components)
        {
            delete comp;
        }
    }

    void Update();

    // Add a component given its data type and arguments to pass to the constructor
    template<typename T, typename... Args>
    inline void AddComponent(Args... constructorArgs)
    {
        components.push_back((component*)(new T(constructorArgs)));
    }

    // Add an already-made component
    inline void AddComponent(component* comp)
    {
        components.push_back(comp);
    }
private:
    std::vector<component*> components;
};
