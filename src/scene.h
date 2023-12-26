#pragma once
#include "Entity.h"

class Scene
{
public:
    Scene();
    void update();

private:
    std::vector<Entity> entities;
};