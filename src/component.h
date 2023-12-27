#pragma once
#include "raylib.h"
#include <iostream>

class Component 
{
public:
    Component()
    {
    }
    virtual void OnAdd(class Entity& parentEntity)
    {
    }
    virtual void Update(class Entity& parentEntity) = 0;
    virtual ~Component()
    {
    }
};
