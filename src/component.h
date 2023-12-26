#pragma once
#include "Entity.h"

class Component 
{
public:
    Component();
    virtual void Update();
    virtual ~Component();
};
