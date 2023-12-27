#include "PhysicsComponent.h"
#include <iostream>

PhysicsComponent::PhysicsComponent(float mass_, Vector2 velocity_, Vector2 acceleration_) 
{
    mass = mass_;
    velocity = velocity_;
    acceleration = acceleration_;
    std::cout<<velocity.x<<std::endl;
}

void PhysicsComponent::Update(Entity& parentEntity) {
    velocity = Vector2Add(velocity, acceleration);
    parentEntity.pos = Vector2Add(parentEntity.pos, velocity);
    // std::cout<<"("<<velocity.x<<", "<<velocity.y<<")"<<std::endl;
    // std::cout<<"("<<parentEntity.pos.x<<", "<<parentEntity.pos.y<<")"<<std::endl;
}