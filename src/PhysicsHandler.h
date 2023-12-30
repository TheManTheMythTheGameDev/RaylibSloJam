#pragma once

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
    }
    void CalculateSteps(float dTime);
    void Step();
    void ResolveCollision(PhysicsComponent* entity1, PhysicsComponent* entity2);
    void AddEntity(PhysicsComponent* newEntity);
    void RemoveEntity(PhysicsComponent* entity);
    void ClearEntities();
    ~PhysicsHandler()
    {
    }

    void DebugDraw();

private:
    std::vector<PhysicsComponent*> physicsEntities;
};
