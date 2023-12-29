#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    Texture2D texture;

    GraphicsComponent(Texture2D texture_ = LoadTexture("src/vecteezy_white-circle-png_21115771_475.png"))
    {
        texture = texture_;
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
