#pragma once
#include "Component.h"
#include "Entity.h"

class GraphicsComponent : public Component
{
public:
    GraphicsComponent()
    {
    }
    ~GraphicsComponent()
    {
    }

    void Update(Entity& parentEntity);
};
