#pragma once
#include "Component.h"
#include <vector>

class Entity
{
public:
    Entity()
    {
    }
    ~Entity()
    {
        for (Component* comp : components)
        {
            delete comp;
        }
    }

    void Update();

    // Add a component given its data type and arguments to pass to the constructor
    template<typename T, typename... Args>
    inline void AddComponent(Args... constructorArgs)
    {
        components.push_back((Component*)(new T(constructorArgs...)));
    }

    // Add an already-made component
    inline void AddComponent(Component* comp)
    {
        components.push_back(comp);
    }
private:
    std::vector<Component*> components;
};
