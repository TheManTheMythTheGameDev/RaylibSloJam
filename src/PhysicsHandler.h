#pragma once

#include "PhysicsComponent.h"

class PhysicsHandler
{
public:
    static constexpr float gConstant = 1.0f;
    static constexpr float stepTime = 1.0f / 60.0f;
    float currentTime;

    PhysicsHandler()
    {
        currentTime = 0;
    }
    void CalculateSteps(float dTime);
    void Step();
    void AddEntity(PhysicsComponent* newEntity);
    void ClearEntities();
    ~PhysicsHandler()
    {
    }

private:
    std::vector<PhysicsComponent*> physicsEntities;
};