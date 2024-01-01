#include "PhysicsHandler.h"

void PhysicsHandler::CalculateSteps(float dTime) {
    currentTime += dTime;
    while (currentTime > stepTime) {
        Step();
        currentTime -= stepTime;
    }
}

void PhysicsHandler::Step() {
    std::vector<PhysicsComponent*> allEntities = spatialPartition.GetAllEntities();
    for (PhysicsComponent* physicsEnt : allEntities)
    {
        physicsEnt->oldPosition = physicsEnt->position;
    }
    spatialPartition.PerformInteractions(PhysicsInteraction);
    spatialPartition.CheckAndCollapseChildren();
    allEntities = spatialPartition.GetAllEntities();
    for (PhysicsComponent* physicsEnt : allEntities) 
    {
        if (physicsEnt->mobile)
        {
            physicsEnt->velocity = Vector2Add(physicsEnt->velocity, Vector2Scale(physicsEnt->acceleration, stepTime));
            physicsEnt->position = Vector2Add(physicsEnt->position, Vector2Scale(physicsEnt->velocity, stepTime));
        }
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
        if (physicsEnt->oldPosition.x != physicsEnt->position.x || physicsEnt->oldPosition.y != physicsEnt->position.y)
        {
            spatialPartition.Update(physicsEnt);
        }
    }
}

void PhysicsHandler::Bounce(PhysicsComponent* mobileEntity, PhysicsComponent* staticEntity)
{
    Vector2 normalUnitVector = Vector2Normalize(Vector2Subtract(staticEntity->position, mobileEntity->position));
    Vector2 normalVelocity = Vector2Scale(normalUnitVector, Vector2DotProduct(mobileEntity->velocity, normalUnitVector));
    Vector2 tangentVelocity = Vector2Subtract(mobileEntity->velocity, normalVelocity);
    mobileEntity->velocity = Vector2Add(Vector2Scale(normalVelocity, -1), tangentVelocity);
    float overlapDist = (mobileEntity->GetShape().shapeData.circleRadius + staticEntity->GetShape().shapeData.circleRadius - Vector2Distance(mobileEntity->position, staticEntity->position));
    mobileEntity->position = Vector2Subtract(mobileEntity->position, Vector2Scale(normalUnitVector, overlapDist));
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
    spatialPartition.Insert(newEntity);
}

void PhysicsHandler::RemoveEntity(PhysicsComponent* entity)
{
    spatialPartition.Remove(entity);
}

void PhysicsHandler::ClearEntities() {
    // TODO: implement
}

void PhysicsHandler::DebugDraw()
{
    spatialPartition.DebugDraw();
}

void PhysicsHandler::PhysicsInteraction(PhysicsComponent* entity1, PhysicsComponent* entity2)
{
    if (entity1->GetShape().shapeType == Shape::ShapeType::Circle && entity2->GetShape().shapeType == Shape::ShapeType::Circle)
    {
        if (entity1->GetShape().shapeData.circleRadius + entity2->GetShape().shapeData.circleRadius > Vector2Distance(entity1->position, entity2->position))
        {
            ResolveCollision(entity1, entity2);
        }
    }
}
