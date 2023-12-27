#include "Scene.h"

void Scene::Update() 
{
    for (Entity ent : entities) {
        ent.Update();
    }
}

void Scene::AddEntity(Entity newEntity) {
    entities.push_back(newEntity);
}