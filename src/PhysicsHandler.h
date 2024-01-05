#pragma once
#include "Quadtree.h"
#include "PhysicsComponent.h"

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
    static bool pointInBox(Vector2 pointPos, Vector2 rectPos, float width, float length, float rotation);
    static float DistanceToLineSegment(Vector2 point, Vector2 corner1, Vector2 corner2);
    static void Bounce(class PhysicsComponent* mobileEntity, Vector2 normalUnitVector);
    static void ResolveCircleCollision(class PhysicsComponent* entity1, class PhysicsComponent* entity2);
    void AddEntity(class PhysicsComponent* newEntity);
    void RemoveEntity(class PhysicsComponent* entity);
    void ClearEntities();
    ~PhysicsHandler()
    {
    }

    void DebugDraw();

private:
    Quadtree spatialPartition;

    static void PhysicsInteraction(PhysicsComponent* entity1, PhysicsComponent* entity2);
};
