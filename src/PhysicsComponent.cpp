#include "PhysicsComponent.h"
#include "PhysicsHandler.h"

PhysicsComponent::PhysicsComponent(PhysicsHandler& physicsHandler_, Shape shape_, float mass_, Vector2 velocity_, Vector2 acceleration_) 
{
    shape = shape_;
    mass = mass_;
    velocity = velocity_;
    acceleration = acceleration_;
    physicsHandler = &physicsHandler_;
}

PhysicsComponent::~PhysicsComponent() 
{
}

void PhysicsComponent::OnAdd(Entity& parentEntity)
{
    position = parentEntity.pos;
    oldPosition = parentEntity.pos;
    physicsHandler->AddEntity(this);
}

void PhysicsComponent::Update(Entity& parentEntity) {
    parentEntity.pos = position;
}