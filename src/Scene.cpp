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
    physicsHandler.CalculateSteps(GetFrameTime());
    for (Entity& ent : entities) {
        ent.Update();
    }
    physicsHandler.DebugDraw();

    // Add deferred entities LAST
    entities.insert(entities.end(), entitiesToAdd.begin(), entitiesToAdd.end());
    entitiesToAdd.clear();
}

void Scene::AddEntity(Entity newEntity) {
    entities.push_back(newEntity);
}

void Scene::DeferAddEntity(Entity newEntity)
{
    entitiesToAdd.push_back(newEntity);
}