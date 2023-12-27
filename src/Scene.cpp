#include "Scene.h"

Scene::~Scene()
{
    for (Entity& ent : entities)
    {
        ent.Unload();
    }
}

void Scene::Update() 
{
    for (Entity& ent : entities) {
        ent.Update();
    }
}

void Scene::AddEntity(Entity newEntity) {
    entities.push_back(newEntity);
}