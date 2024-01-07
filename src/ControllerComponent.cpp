// #pragma once
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "raylib.h"
#include <math.h>
#include "raymath.h"
#include "GraphicsComponent.h"
#include "FragileComponent.h"

void ControllerComponent::Update(Entity& parentEntity)
{
    if (isActive) {
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

        if (IsKeyPressed(KEY_SPACE))
        {
            float bulletDist = physicsComp->GetShape().shapeData.circleRadius + bulletRadius + 0.1f;
            float radianAngle = angle * DEG2RAD;
            Vector2 bulletPos = { physicsComp->position.x + bulletDist * cosf(radianAngle), physicsComp->position.y + bulletDist * sinf(radianAngle) };
            std::cout << bulletSpeed << std::endl;
            Vector2 bulletVelocity = { bulletSpeed * cosf(radianAngle), bulletSpeed * sinf(radianAngle) };
            Entity* bullet = new Entity(bulletPos, radianAngle, 0, new GraphicsComponent(bulletTexture, bulletRadius, bulletRadius), new PhysicsComponent(scene->physicsHandler, Shape(Shape::ShapeType::Circle, Shape::ShapeData{ bulletRadius }), true, 1.0f, bulletVelocity), new FragileComponent(scene));
            scene->DeferAddEntity(bullet);
        }

        Vector2 thrustVector = Vector2Scale(Vector2{ cosf(angle * DEG2RAD), sinf(angle * DEG2RAD) }, thrustAccumulator);
        physicsComp->velocity = Vector2Add(physicsComp->velocity, thrustVector);

        if (Vector2Length(physicsComp->velocity) > 1000)
            physicsComp->velocity = Vector2Scale(Vector2Normalize(physicsComp->velocity), 1000);

        DrawLine(parentEntity.pos.x, parentEntity.pos.y, parentEntity.pos.x + cosf(angle * DEG2RAD) * 100.0f, parentEntity.pos.y + sinf(angle * DEG2RAD) * 100.0f, BLUE);
    }
}
