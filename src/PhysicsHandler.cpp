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
        // physicsEnt->velocity = Vector2Scale(Vector2Add(physicsEnt->velocity, physicsEnt->acceleration), stepTime);
        // physicsEnt->position = Vector2Scale(Vector2Add(physicsEnt->position, physicsEnt->velocity), stepTime);
    }
}

void PhysicsHandler::AddEntity(PhysicsComponent* newEntity) {
    physicsEntities.push_back(newEntity);
}

void PhysicsHandler::ClearEntities() {
    physicsEntities.clear();
}
