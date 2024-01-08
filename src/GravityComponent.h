#pragma once
#include "CollisionListenComponent.h"

class GravityComponent : public CollisionListenComponent
{
public:
    GravityComponent(int listeningTag_ = 0, int blacklistTag_ = 7) : CollisionListenComponent(listeningTag_)
    {
        blacklistTag = blacklistTag_;
    }
    void OnTriggerCollision(Entity* otherEntity, Entity* thisEntity);
    ~GravityComponent()
    {
    }

private:
    int blacklistTag;
    static constexpr float gravitationalConstant = 100.0f;
};
