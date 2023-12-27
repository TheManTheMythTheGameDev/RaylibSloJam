#pragma once

#include "PhysicsComponent.h"

class PhysicsHandler
{
public:
    static const float gConstant = 1.0;
    static const float stepTime = 1.0 / 60.0;
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