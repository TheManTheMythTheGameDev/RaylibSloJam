#pragma once
#include "Entity.h"

class CollisionListenComponent : public Component
{
public:
    CollisionListenComponent(int listeningTag_)
    {
        listeningTag = listeningTag_;
    }
    void CheckCollision(std::vector<Entity*> collisions)
    {
        for (Entity* collision : collisions)
        {
            if (collision->tag.test(listeningTag))
            {
                OnTriggerCollision(collision);
            }
        }
    }
    virtual void OnTriggerCollision(Entity* otherEntity);
    ~CollisionListenComponent()
    {
    }

protected:
    int listeningTag;
};