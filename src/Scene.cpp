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
}

void Scene::AddEntity(Entity newEntity) {
    entities.push_back(newEntity);
}