#include "FragileComponent.h"
#include <iostream>

void FragileComponent::OnTriggerCollision(Entity* otherEntity, Entity* thisEntity)
{
    currentScene->DeferDeleteEntity(thisEntity);
}
