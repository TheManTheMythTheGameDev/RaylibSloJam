#pragma once
#include "Entity.h"
#include "raymath.h"
#include "Shape.h"

class PhysicsComponent : public Component
{
public:
    bool mobile;
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Entity* holdingEntity;
    std::vector<Entity*> previousCollided;
    std::vector<Entity*> currentCollided;

    PhysicsComponent(class PhysicsHandler& physicsHandler, Shape shape_, bool mobile_ = false, float mass_ = 1, Vector2 velocity_ = Vector2{ 0.0f, 0.0f }, Vector2 acceleration_ = Vector2{ 0.0f, 0.0f });
    ~PhysicsComponent();
    void OnAdd(Entity& parentEntity);
    void Update(Entity& parentEntity);

    inline Shape GetShape()
    {
        return shape;
    }
private:
    Shape shape;
};
