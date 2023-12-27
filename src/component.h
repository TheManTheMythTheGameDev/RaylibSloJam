#pragma once
#include "raylib.h"

class Component 
{
public:
    Component();
    virtual void Update(Vector2 entityPos);
    virtual ~Component();
};
