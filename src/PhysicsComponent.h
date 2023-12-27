#pragma once

#include "Entity.h"
#include "raymath.h"

class PhysicsComponent : public Component
{
public:
    float mass;
    Vector2 velocity;
    Vector2 acceleration;

    PhysicsComponent(float mass_, Vector2 velocity_, Vector2 acceleration_);
    PhysicsComponent(float mass_, Vector2 velocity_);
    PhysicsComponent(float mass_);
    PhysicsComponent();
    void Update(Entity& parentEntity);
    ~PhysicsComponent()
    {
    }
};