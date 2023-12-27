#pragma once
#include "Entity.h"

class Scene
{
public:
    Scene()
    {
    }
    ~Scene();
    void Update();
    void AddEntity(Entity newEntity);

private:
    std::vector<Entity> entities;
};