#include "PhysicsComponent.h"

void PhysicsComponent::Update(Entity& parent) {
    velocity = Vector2Add(velocity, acceleration);
    parent.pos = Vector2Add(parent.pos, velocity);
}