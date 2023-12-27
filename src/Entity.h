#pragma once
#include "Component.h"
#include <vector>
#include "raylib.h"

class Entity
{
public:
    Entity(Vector2 _pos = Vector2{ 0.0f, 0.0f })
    {
        pos = _pos;
    }
    Entity(Vector2 _pos, std::vector<Component*> _components)
    {
        pos = _pos;
        components = _components;
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

    Vector2 pos;
private:
    std::vector<Component*> components;
};
