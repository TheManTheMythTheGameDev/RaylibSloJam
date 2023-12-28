#include "PhysicsHandler.h"

void PhysicsHandler::CalculateSteps(float dTime) {
    currentTime += dTime;
    while (currentTime > stepTime) {
        Step();
        currentTime -= stepTime;
    }
}

void PhysicsHandler::Step() {
    for (int e1 = 0; e1 < physicsEntities.size() - 1; e1++)
    {
        PhysicsComponent* entity1 = physicsEntities[e1];
        for (int e2 = e1 + 1; e2 < physicsEntities.size(); e2++) 
        {
            PhysicsComponent* entity2 = physicsEntities[e2];
            if (entity1->GetShape().shapeType == Shape::ShapeType::Circle && entity2->GetShape().shapeType == Shape::ShapeType::Circle) 
            {
                if (entity1->GetShape().shapeData.circleRadius + entity2->GetShape().shapeData.circleRadius > Vector2Distance(entity1->position, entity2->position))
                {
                    ResolveCollision(entity1, entity2);
                }
            }
        }
    }
    for (PhysicsComponent* physicsEnt : physicsEntities) 
    {
        physicsEnt->velocity = Vector2Add(physicsEnt->velocity, Vector2Scale(physicsEnt->acceleration, stepTime));
        physicsEnt->position = Vector2Add(physicsEnt->position, Vector2Scale(physicsEnt->velocity, stepTime));
        if (physicsEnt->position.x < -borderLength) 
        {
            physicsEnt->position.x += GetScreenWidth() + 2 * borderLength;
        }
        if (physicsEnt->position.x > GetScreenWidth() + borderLength) 
        {
            physicsEnt->position.x -= GetScreenWidth() + 2 * borderLength;
        }
        if (physicsEnt->position.y < -borderLength) 
        {
            physicsEnt->position.y += GetScreenHeight() + 2 * borderLength;
        }
        if (physicsEnt->position.y > GetScreenHeight() + borderLength) 
        {
            physicsEnt->position.y -= GetScreenHeight() + 2 * borderLength;
        }
    }
}

void PhysicsHandler::ResolveCollision(PhysicsComponent* entity1, PhysicsComponent* entity2)
{
    Vector2 normalUnitVector = Vector2Normalize(Vector2Subtract(entity2->position, entity1->position));
    Vector2 entity1NormalVelocity = Vector2Scale(normalUnitVector, Vector2DotProduct(entity1->velocity, normalUnitVector));
    Vector2 entity1TangentVelocity = Vector2Subtract(entity1->velocity, entity1NormalVelocity);
    Vector2 negativeNormalVector = Vector2Scale(normalUnitVector, -1);
    Vector2 entity2NormalVelocity = Vector2Scale(negativeNormalVector, Vector2DotProduct(entity2->velocity, negativeNormalVector));
    Vector2 entity2TangentVelocity = Vector2Subtract(entity2->velocity, entity2NormalVelocity);
    float combinedMass = entity1->mass + entity2->mass;
    Vector2 entity1CollisionVelocity = Vector2Scale(Vector2Add(Vector2Scale(entity1NormalVelocity, entity1->mass - entity2->mass), Vector2Scale(entity2NormalVelocity, 2 * entity2->mass)), 1 / (combinedMass));
    Vector2 entity2CollisionVelocity = Vector2Scale(Vector2Add(Vector2Scale(entity2NormalVelocity, entity2->mass - entity1->mass), Vector2Scale(entity1NormalVelocity, 2 * entity1->mass)), 1 / (combinedMass));
    entity1->velocity = Vector2Add(entity1CollisionVelocity, entity1TangentVelocity);
    entity2->velocity = Vector2Add(entity2CollisionVelocity, entity2TangentVelocity);
    float overlapDist = (entity1->GetShape().shapeData.circleRadius + entity2->GetShape().shapeData.circleRadius - Vector2Distance(entity1->position, entity2->position)) / 2.0;
    entity1->position = Vector2Subtract(entity1->position, Vector2Scale(normalUnitVector, overlapDist));
    entity2->position = Vector2Subtract(entity2->position, Vector2Scale(negativeNormalVector, overlapDist));
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
