#pragma once
#include "Component.h"
#include "PhysicsComponent.h"
#include <unordered_map>
#include <bitset>
#include "raylib.h"

extern int componentCounter;

template<class T>
inline int GetID()
{
    static int componentID = componentCounter++;
    return componentID;
}

class Entity
{
public:
    std::bitset<8> tag;
    Entity(Vector2 _pos = Vector2{ 0.0f, 0.0f }, float _rotation = 0.0f, int tagIndex = 0)
    {
        pos = _pos;
        rotation = _rotation;
        tag.set(0);
        tag.set(tagIndex);
        ID = entityCounter++;
    }
    template<typename... Ts>
    Entity(Vector2 _pos = Vector2{ 0.0f, 0.0f }, float _rotation = 0.0f, int tagIndex = 0, Ts*... _components)
    {
        pos = _pos;
        rotation = _rotation;
        tag.set(0);
        tag.set(tagIndex);
        int ints[] = { AddComponentInternal(_components)... };
        ID = entityCounter++;
    }
    void Unload();

    void Update();

    // Add a component given its data type and arguments to pass to the constructor
    template<typename T, typename... Args>
    inline void AddComponent(Args... constructorArgs)
    {
        Component* comp = (Component*)(new T(constructorArgs...));
        components[GetID<T>()] = comp;
        comp->OnAdd(*this);
    }

    // Add an already-made component
    template<typename T>
    inline void AddComponent(T* comp)
    {
        // std::cout<<static_cast<PhysicsComponent*>(comp)->velocity.x<<std::endl;
        int ID = GetID<T>();
        components[ID] = static_cast<Component*>(comp);
        comp->OnAdd(*this);
    }

    // Grab a component of a given type
    template<typename T>
    T* GetComponent()
    {
        return dynamic_cast<T*>(components[GetID<T>()]);
    }

    Vector2 pos;
    float rotation;

    // There's no way to know for sure whether two entities are the same unless we implement an ID system
    bool operator==(const Entity& other)
    {
        return (ID == other.ID);
    }
private:
    std::unordered_map<int, Component*> components;
    unsigned int ID;

    // Add an already-made component
    template<typename T>
    inline int AddComponentInternal(T* comp)
    {
        int ID = GetID<T>();
        components[ID] = static_cast<Component*>(comp);
        comp->OnAdd(*this);
        return ID;
    }

    static unsigned int entityCounter;
};
