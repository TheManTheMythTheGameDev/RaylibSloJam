#pragma once
#include "Component.h"
#include "scene.h"
#include "raylib.h"

class DestructibleWallComponent : public Component
{
public:
    DestructibleWallComponent(Scene& _scene, Texture2D _circleTex, Vector2 _dims);
    ~DestructibleWallComponent();

    void Update(class Entity& parentEntity);

    static constexpr int minCircles = 7;
    static constexpr int maxCircles = 9;
private:
    // I would make a separate module for this but realistically only DestructibleWallComponent is gonna need it
    struct Circle
    {
        Vector2 pos;
        float radius;
    };
    Circle* PackCircles(int numCircles, Vector2 pos);

    Scene& scene;
    Texture2D circleTex;
    Vector2 dims;
};
