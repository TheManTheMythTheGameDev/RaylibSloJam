#include "Scene.h"

Scene::~Scene()
{
    for (Entity* ent : entities)
    {
        ent->Unload();
        delete ent;
    }
}

void Scene::Update()
{
    physicsHandler.CalculateSteps(GetFrameTime());
    for (Entity* ent : entities) {
        ent->Update();
    }
    physicsHandler.DebugDraw();

    // Add & delete deferred entities LAST
    for (Entity* ent : entitiesToDelete)
    {
        ent->Unload();
        delete ent;
        entities.erase(std::remove(entities.begin(), entities.end(), ent));
    }
    entitiesToDelete.clear();

    entities.insert(entities.end(), entitiesToAdd.begin(), entitiesToAdd.end());
    entitiesToAdd.clear();
}

void Scene::AddEntity(Entity* newEntity) {
    entities.push_back(newEntity);
}

void Scene::DeferAddEntity(Entity* newEntity)
{
    entitiesToAdd.push_back(newEntity);
}

void Scene::DeferDeleteEntity(Entity* entity)
{
    entitiesToDelete.push_back(entity);
}
