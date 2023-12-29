#include "GraphicsComponent.h"

void GraphicsComponent::Update(Entity& parentEntity)
{
    // DrawCircle(parentEntity.pos.x, parentEntity.pos.y, 100.0f, RED);
    DrawTexturePro(
        texture,
        Rectangle{0, 0, float(texture.width), float(texture.height)},
        Rectangle{parentEntity.pos.x, parentEntity.pos.y, 100.0f * 2, 100.0f * 2},
        Vector2{100.0f, 100.0f},
        0, RED);
}
