#include "Entity.h"

void Entity::Update()
{
    for (component* comp : components)
    {
        comp->update();
    }
}
