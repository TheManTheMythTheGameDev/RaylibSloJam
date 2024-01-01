#pragma once
#include "PhysicsComponent.h"
#include "Quadtree.h"

class PhysicsHandler
{
public:
    static constexpr float gConstant = 1.0f;
    static constexpr float stepTime = 1.0f / 60.0f;
    static constexpr float borderLength = 100;
    float currentTime;

    PhysicsHandler()
    {
        currentTime = 0;
        spatialPartition = Quadtree(1, Vector2{ -borderLength, -borderLength }, Vector2{ (float)GetScreenWidth() + borderLength * 2.0f, (float)GetScreenHeight() + borderLength * 2.0f});
    }
    void CalculateSteps(float dTime);
    void Step();
    static void ResolveCollision(PhysicsComponent* entity1, PhysicsComponent* entity2);
    void AddEntity(PhysicsComponent* newEntity);
    void RemoveEntity(PhysicsComponent* entity);
    void ClearEntities();
    ~PhysicsHandler()
    {
    }

    void DebugDraw();

private:
    Quadtree spatialPartition;

    static void PhysicsInteraction(PhysicsComponent* entity1, PhysicsComponent* entity2);
};
