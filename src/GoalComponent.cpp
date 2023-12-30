#include "GoalComponent.h"

void GoalComponent::OnTriggerCollision(Entity* otherEntity)
{
    player->isActive = false;
    currentScene->AddEntity(Entity(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 0, new GraphicsComponent(endScreen)));
}