#pragma once
#include "Component.h"
#include <vector>

class Entity
{
public:
    Entity();
    ~Entity();
private:
    std::vector<Component*> components;
};
