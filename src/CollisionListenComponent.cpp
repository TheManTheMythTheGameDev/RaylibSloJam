#include "CollisionListenComponent.h"

void CollisionListenComponent::Update(Entity& parentEntity)
{
    for (Entity* collision : parentEntity.GetComponent<PhysicsComponent>()->previousCollided)
    {
        if (collision->tag.test(listeningTag))
        {
            OnTriggerCollision(collision, &parentEntity);
        }
    }
}
