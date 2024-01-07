#include "GoalComponent.h"

void GoalComponent::OnTriggerCollision(Entity* otherEntity)
{
    player->isActive = false;
    currentScene->DeferAddEntity(Entity(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 0.0f, 0, new GraphicsComponent(endScreen, 400.0f, 400.0f)));
}