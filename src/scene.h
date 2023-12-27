#pragma once
#include "Entity.h"

class Scene
{
public:
    Scene();
    void Update();
    void AddEntity(Entity newEntity);

private:
    std::vector<Entity> entities;
};