#include "PhysicsComponent.h"

void PhysicsComponent::Update(Entity& parent) {
    velocity = Vector2Add(velocity, acceleration);
    parent.position = Vector2Add(parent.position, velocity);
}