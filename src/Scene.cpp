#include "Scene.h"

void Scene::Update() 
{
    for (Entity& ent : entities) {
        ent.Update();
    }
}