#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    Texture2D texture;
    float width;
    float height;
    float rotation;

    GraphicsComponent(Texture2D texture_, float width_ = 20.0f, float height_ = 20.0f, float rotation_ = 0.0f)
    {
        texture = texture_;
        width = width_;
        height = height_;
        rotation = rotation_;
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
