#include "PhysicsComponent.h"
#include "PhysicsHandler.h"

PhysicsComponent::PhysicsComponent(PhysicsHandler& physicsHandler, float mass_, Vector2 velocity_, Vector2 acceleration_) 
{
    mass = mass_;
    velocity = velocity_;
    acceleration = acceleration_;
    physicsHandler.AddEntity(this);
}

PhysicsComponent::~PhysicsComponent() 
{
}

void PhysicsComponent::OnAdd(Entity& parentEntity)
{
    position = parentEntity.pos;
}

void PhysicsComponent::Update(Entity& parentEntity) {
    parentEntity.pos = position;
    // velocity = Vector2Add(velocity, acceleration);
    // parentEntity.pos = Vector2Add(parentEntity.pos, velocity);
    // std::cout<<"("<<velocity.x<<", "<<velocity.y<<")"<<std::endl;
    // std::cout<<"("<<parentEntity.pos.x<<", "<<parentEntity.pos.y<<")"<<std::endl;
}