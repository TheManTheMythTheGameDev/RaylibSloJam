#pragma once
#include "CollisionListenComponent.h"
#include "ControllerComponent.h"
#include "GraphicsComponent.h"
#include "Scene.h"

class GoalComponent : public CollisionListenComponent
{
public:
    GoalComponent(ControllerComponent* player_, Scene* currentScene_, Texture2D endScreen_) : CollisionListenComponent(1)
    {
        player = player_;
        currentScene = currentScene_;
        endScreen = endScreen_;
    }
    void OnTriggerCollision(Entity* otherEntity);
    ~GoalComponent()
    {
    }

private:
    ControllerComponent* player;
    Scene* currentScene;
    Texture2D endScreen;
};