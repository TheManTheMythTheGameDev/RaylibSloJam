#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(float mass_, Vector2 velocity_, Vector2 acceleration_) 
{
    mass = mass_;
    velocity = velocity_;
    acceleration = acceleration_;
    std::cout<<velocity.x<<std::endl;
}

PhysicsComponent::PhysicsComponent(float mass_, Vector2 velocity_) 
{
    std::cout<<velocity_.x<<std::endl;
    PhysicsComponent(mass_, velocity_, Vector2{0, 0});
}

PhysicsComponent::PhysicsComponent(float mass_) 
{
    PhysicsComponent(mass_, Vector2{0, 0}, Vector2{0, 0});
}

PhysicsComponent::PhysicsComponent() 
{
    PhysicsComponent(1, Vector2{0, 0}, Vector2{0, 0});
}

void PhysicsComponent::Update(Entity& parentEntity) {
    velocity = Vector2Add(velocity, acceleration);
    parentEntity.pos = Vector2Add(parentEntity.pos, velocity);
    // std::cout<<"("<<velocity.x<<", "<<velocity.y<<")"<<std::endl;
    // std::cout<<"("<<parentEntity.pos.x<<", "<<parentEntity.pos.y<<")"<<std::endl;
}