#include "PhysicsHandler.h"

void PhysicsHandler::CalculateSteps(float dTime) {
    currentTime += dTime;
    while (currentTime > stepTime) {
        Step();
        currentTime -= stepTime;
    }
}

void PhysicsHandler::Step() {
    for (PhysicsComponent* physicsEnt : physicsEntities) {
        physicsEnt->velocity = Vector2{ 5, 5 };
    }
}

void PhysicsHandler::AddEntity(PhysicsComponent* newEntity) {
    physicsEntities.push_back(newEntity);
}

void PhysicsHandler::RemoveEntity(PhysicsComponent* entity)
{
    physicsEntities.erase(std::remove(physicsEntities.begin(), physicsEntities.end(), entity));
}

void PhysicsHandler::ClearEntities() {
    int numEntities = physicsEntities.size();
    for (int i = 0; i < numEntities; i++) {
        physicsEntities.pop_back();
    }
}
