#pragma once
#include "Entity.h"
#include "PhysicsHandler.h"

class Scene
{
public:
    Scene(PhysicsHandler physicsHandler_ = PhysicsHandler())
    {
        physicsHandler = physicsHandler_;
    }
    ~Scene();
    void Update();
    void AddEntity(Entity newEntity);

    PhysicsHandler physicsHandler;
private:
    std::vector<Entity> entities;
};
