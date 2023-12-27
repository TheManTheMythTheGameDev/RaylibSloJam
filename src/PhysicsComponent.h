#pragma once

#include "Entity.h"
#include "raymath.h"

class PhysicsComponent : public Component
{
public:
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;

    PhysicsComponent(class PhysicsHandler& physicsHandler, float mass_ = 1, Vector2 velocity_ = Vector2{ 0.0f, 0.0f }, Vector2 acceleration_ = Vector2{ 0.0f, 0.0f });
    void OnAdd(Entity& parentEntity);
    void Update(Entity& parentEntity);
    ~PhysicsComponent()
    {
    }
};