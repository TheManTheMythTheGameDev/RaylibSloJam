#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    Texture2D texture;

    GraphicsComponent(Texture2D texture_)
    {
        texture = texture_;
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
