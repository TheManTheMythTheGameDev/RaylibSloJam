#pragma once

#include "Entity.h"

class PhysicsComponent : public Entity
{
public:
    float mass;
    Vector2 velocity;
    Vector2 acceleration;

    PhysicsComponent(float mass_, Vector2 velocity_)
    {
        mass = mass_;
        velocity = velocity_;
        acceleration = Vector2{0, 0};
    }
    PhysicsComponent(float mass_)
    {
        PhysicsComponent(mass_, Vector2{0, 0});
    }
    PhysicsComponent()
    {
        PhysicsComponent(1, Vector2{0, 0});
    }
    void Update();
    ~PhysicsComponent();
};