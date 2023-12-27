#pragma once
#include "Component.h"
#include <unordered_map>
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
    Vector2 position;
    
    Entity(Vector2 _pos = Vector2{ 0.0f, 0.0f })
    {
        pos = _pos;
    }
    template<typename... Ts>
    Entity(Vector2 _pos, Ts*... _components)
    {
        pos = _pos;
        AddComponent<Ts...>(_components...);
    }
    void Unload();

    void Update();

    // Add a component given its data type and arguments to pass to the constructor
    /*template<typename T, typename... Args>
    inline void AddComponent(Args... constructorArgs)
    {
        components[GetID<T>()] = (Component*)(new T(constructorArgs...));
    }*/

    // Add an already-made component
    template<typename T>
    inline void AddComponent(T* comp)
    {
        int ID = GetID<T>();
        components[ID] = comp;
    }

    Vector2 pos;
private:
    std::unordered_map<int, Component*> components;
};
