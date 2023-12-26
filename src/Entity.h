#pragma once
#include "component.h"
#include <vector>

class Entity
{
public:
    Entity();
    ~Entity();


private:
    std::vector<component*> components;
};
