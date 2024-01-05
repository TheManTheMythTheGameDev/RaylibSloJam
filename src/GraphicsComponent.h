#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    Texture2D texture;
    float radius;

    GraphicsComponent(Texture2D texture_, float radius_ = 20.0f)
    {
        texture = texture_;
        radius = radius_;
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
