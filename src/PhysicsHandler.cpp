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
        physicsEnt->velocity = Vector2Add(physicsEnt->velocity, Vector2Scale(physicsEnt->acceleration, stepTime));
        physicsEnt->position = Vector2Add(physicsEnt->position, Vector2Scale(physicsEnt->velocity, stepTime));
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
    physicsEntities.clear();
}
