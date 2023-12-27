#pragma once
#include "raylib.h"

class Component 
{
public:
    Component()
    {
    }
    virtual void Update(class Entity& parentEntity) = 0;
    virtual ~Component()
    {
    }
};
