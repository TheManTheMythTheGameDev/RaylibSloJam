#pragma once
#include "Entity.h"
#include "raymath.h"
#include "Shape.h"
#include <vector>

class PhysicsComponent : public Component
{
public:
    bool mobile;
    float mass;
    float rotation;
    Vector2 oldPosition;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    Entity* holdingEntity;
    std::vector<Entity*> previousCollided;
    std::vector<Entity*> currentCollided;
    std::bitset<8> collisionBlacklist;

    PhysicsComponent(class PhysicsHandler& physicsHandler_, Shape shape_, bool mobile_ = false, int blackList = -1, float mass_ = 1.0f, Vector2 velocity_ = Vector2{ 0.0f, 0.0f }, Vector2 acceleration_ = Vector2{ 0.0f, 0.0f });
    ~PhysicsComponent();
    void OnAdd(Entity& parentEntity);
    void Update(Entity& parentEntity);

    inline Shape GetShape()
    {
        return shape;
    }
private:
    Shape shape;
    PhysicsHandler* physicsHandler;
};
