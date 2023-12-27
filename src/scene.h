#pragma once
#include "Entity.h"
#include "PhysicsHandler.h"

class Scene
{
public:
    Scene(PhysicsHandler physicsHandler_)
    {
        physicsHandler = physicsHandler_;
    }
    ~Scene();
    void Update();
    void AddEntity(Entity newEntity);

private:
    PhysicsHanlder physicsHandler;
    std::vector<Entity> entities;
};