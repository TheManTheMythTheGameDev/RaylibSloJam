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
    // Use this function only if an entity is being added at the very beginnning/end of a frame
    void AddEntity(Entity* newEntity);
    // Use this function if an entity is being added in the middle of a frame -- i.e. in a component's update function
    void DeferAddEntity(Entity* newEntity);
    // Theoretically entity deletion doesn't need to be deferred but this is just easier to think about
    void DeferDeleteEntity(Entity* entity);

    PhysicsHandler physicsHandler;
private:
    std::vector<Entity*> entities;
    std::vector<Entity*> entitiesToAdd;
    std::vector<Entity*> entitiesToDelete;
};
