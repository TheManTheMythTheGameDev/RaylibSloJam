#pragma once
#include "CollisionListenComponent.h"
#include "Scene.h"

class FragileComponent : public CollisionListenComponent
{
public:
    FragileComponent(Scene* currentScene_) : CollisionListenComponent(0)
    {
        currentScene = currentScene_;
    }
    void OnTriggerCollision(Entity* otherEntity, Entity* thisEntity);
    ~FragileComponent()
    {
    }

private:
    Scene* currentScene;
};
