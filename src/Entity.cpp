#include "Entity.h"

void Entity::Update()
{
    for (Component* comp : components)
    {
        comp->update();
    }
}
