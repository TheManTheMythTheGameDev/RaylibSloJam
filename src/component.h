#pragma once
#include "Entity.h"

class Component 
{
public:
    Component();
    virtual void update();
    virtual ~Component();
};
