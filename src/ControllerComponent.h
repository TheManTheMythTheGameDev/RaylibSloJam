#pragma once
#include "Scene.h"
#include "GraphicsComponent.h"
#include "FragileComponent.h"

class ControllerComponent : public Component
{
public:
    bool isActive;
    Scene* scene;
    Texture2D bulletTexture;

    ControllerComponent(Texture2D bulletTexture_, Scene* scene_)
    {
        isActive = true;
        angle = 0.0f;
        bulletTexture = bulletTexture_;
        scene = scene_;
    }
    ~ControllerComponent()
    {
    }

    void Update(Entity& parentEntity);

    static constexpr float thrustMultiplier = 200.0f;
    static constexpr float rotationMultiplier = 180.0f;
    static constexpr float bulletRadius = 5.0f;
    static constexpr float bulletSpeed = 300.0f;
private:
    float angle;
};
