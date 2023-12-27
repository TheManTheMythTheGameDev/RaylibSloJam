#pragma once
#include "raylib.h"
#include <iostream>

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
