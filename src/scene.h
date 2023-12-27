#pragma once
#include "Entity.h"

class Scene
{
public:
    Scene()
    {
    }
    void Update();

private:
    std::vector<Entity> entities;
};