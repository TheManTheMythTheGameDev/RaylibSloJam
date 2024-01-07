#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    Texture2D texture;
    float width;
    float height;

    GraphicsComponent(Texture2D texture_, float width_ = 20.0f, float height_ = 20.0f)
    {
        texture = texture_;
        width = width_;
        height = height_;
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
