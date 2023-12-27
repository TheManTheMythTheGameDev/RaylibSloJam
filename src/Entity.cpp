#include "Entity.h"

void Entity::Update()
{
    for (Component* comp : components)
    {
        comp->Update(pos);
    }
}
