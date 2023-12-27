#include "Entity.h"

int componentCounter;

void Entity::Unload()
{
    for (auto& comp : components)
    {
        delete comp.second;
    }
}

void Entity::Update()
{
    for (auto& comp : components)
    {
        comp.second->Update(*this);
    }
}
