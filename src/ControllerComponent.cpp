#pragma once
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "raylib.h"
#include <math.h>
#include "raymath.h"

void ControllerComponent::Update(Entity& parentEntity)
{
    PhysicsComponent* physicsComp = parentEntity.GetComponent<PhysicsComponent>();

    float rotationAmount = rotationMultiplier * GetFrameTime();
    float thrustAmount = thrustMultiplier * GetFrameTime();

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        angle -= rotationAmount;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        angle += rotationAmount;
    }

    float thrustAccumulator = 0.0f;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        thrustAccumulator += thrustAmount;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        thrustAccumulator -= thrustAmount;
    }

    Vector2 thrustVector = Vector2Scale(Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) }, thrustAccumulator);
    physicsComp->velocity = Vector2Add(physicsComp->velocity, thrustVector);

    if (Vector2Length(physicsComp->velocity) > 1000)
        physicsComp->velocity = Vector2Scale(Vector2Normalize(physicsComp->velocity), 1000);

    DrawLine(parentEntity.pos.x, parentEntity.pos.y, parentEntity.pos.x + cosf(angle * DEG2RAD) * 100.0f, parentEntity.pos.y + sinf(angle * DEG2RAD) * 100.0f, BLUE);
}
