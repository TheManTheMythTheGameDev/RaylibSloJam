#include "GravityComponent.h"

void GravityComponent::OnTriggerCollision(Entity* otherEntity, Entity* thisEntity)
{
    if (!otherEntity->tag.test(blacklistTag))
    {
        PhysicsComponent* otherEntityPhysics = otherEntity->GetComponent<PhysicsComponent>();
        PhysicsComponent* thisEntityPhysics = thisEntity->GetComponent<PhysicsComponent>();
        Vector2 gravityVector = Vector2Normalize(Vector2Subtract(thisEntityPhysics->position, otherEntityPhysics->position));
        float gravityForce = gravitationalConstant * thisEntityPhysics->mass * otherEntityPhysics->mass / pow(Vector2Distance(thisEntityPhysics->position, otherEntityPhysics->position), 2);
        std::cout << gravityForce << std::endl;
        gravityVector = Vector2Scale(gravityVector, gravityForce);
        otherEntityPhysics->velocity = Vector2Add(otherEntityPhysics->velocity, Vector2Scale(gravityVector, 1 / otherEntityPhysics->mass));
    }
}