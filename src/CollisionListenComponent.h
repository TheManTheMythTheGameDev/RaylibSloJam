#pragma once
#include "Entity.h"
#include "PhysicsComponent.h"

class CollisionListenComponent : public Component
{
public:
    CollisionListenComponent(int listeningTag_)
    {
        listeningTag = listeningTag_;
    }
    virtual void Update(Entity& parentEntity);
    virtual void OnTriggerCollision(Entity* otherEntity, Entity* thisEntity) = 0;
    ~CollisionListenComponent()
    {
    }

protected:
    int listeningTag;
};