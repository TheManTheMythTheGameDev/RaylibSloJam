#pragma once
#include "CollisionListenComponent.h"
#include "Scene.h"

class FragileComponent : public CollisionListenComponent
{
public:
    FragileComponent(Scene* currentScene_) : CollisionListenComponent(0)
    {
        currentScene = currentScene_;
        holdingEntity = Entity();
    }
    void OnAdd(Entity& parentEntity)
    {
        holdingEntity = parentEntity;
    }
    void OnTriggerCollision(Entity* otherEntity)
    {
        currentScene->DeferDeleteEntity(holdingEntity);
    }
    ~FragileComponent()
    {
    }

private:
    Scene* currentScene;
    Entity& holdingEntity;
};