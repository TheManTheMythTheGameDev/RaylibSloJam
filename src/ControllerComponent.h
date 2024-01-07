#pragma once
#include "Component.h"
#include "Entity.h"

class ControllerComponent : public Component
{
public:
    bool isActive;
    Scene* scene;
    Texture2D projectileTexture;

    ControllerComponent(Texture2D projectileTexture_, Scene* scene_)
    {
        isActive = true;
        angle = 0.0f;
        projectileTexture = projectileTexture_;
        scene = scene_;
    }
    ~ControllerComponent()
    {
    }

    void Update(Entity& parentEntity);

    static constexpr float thrustMultiplier = 200.0f;
    static constexpr float rotationMultiplier = 180.0f;
private:
    float angle;
};
