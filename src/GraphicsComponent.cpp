#include "GraphicsComponent.h"

void GraphicsComponent::Update(Entity& parentEntity)
{
    DrawCircle(parentEntity.pos.x, parentEntity.pos.y, 100.0f, RED);
}
