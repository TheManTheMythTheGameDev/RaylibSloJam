#pragma once
#include "Component.h"
#include "Entity.h"

class ControllerComponent : public Component
{
public:
    bool isActive;

    ControllerComponent()
    {
        isActive = true;
        angle = 0.0f;
    }
    ~ControllerComponent()
    {
    }

    void Update(Entity& parentEntity);

    static constexpr float thrustMultiplier = 300.0f;
    static constexpr float rotationMultiplier = 180.0f;
private:
    float angle;
};
