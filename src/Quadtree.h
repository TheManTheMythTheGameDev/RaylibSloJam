#pragma once
#include "Entity.h"

class Quadtree
{
public:
    bool alreadySplit;
    int capacity;
    int level;
    Vector2 position;
    Vector2 dimensions;
    std::vector<PhysicsComponent*> entities;
    Quadtree* branches[4];
    enum BranchType
    {
        NW = 0,
        NE,
        SW,
        SE
    };

    Quadtree(int capacity_ = 1, Vector2 position_ = Vector2{0, 0}, Vector2 dimensions_ = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() }, int level_ = 0)
    {
        alreadySplit = false;
        capacity = capacity_;
        position = position_;
        dimensions = dimensions_;
        for (int i = 0; i < 4; i++)
        {
            branches[i] = nullptr;
        }
        level = level_;
    }
    ~Quadtree();
    void Split();
    void Insert(PhysicsComponent* newEntity);
    // Remove an entity -- one moment it's there, the next it's gone
    // Does the same as Insert, but you know... the opposite
    void Remove(PhysicsComponent* entity);
    // We need the old position so we can know which quads it's stored in
    void Update(PhysicsComponent* changedEntity, Vector2 oldPosition);

    // Draw some nice rectangles for debugging purposes
    void DebugDraw();

    static constexpr int maxLevel = 5;
private:
    size_t TallyEntities();
    std::vector<PhysicsComponent*> GetAllEntities();
    // Used by Remove()
    void IRemove(PhysicsComponent* entity);
    // I don't feel like querying for quads that might have to be updated -- screw it, just update 'em all
    // Should be very cheap (it doesn't do anything!) if it doesn't need to be updated, although ideally this function should be avoided...
    void UpdateAll();
    // Find where an entity should be
    Quadtree* FindSlot(PhysicsComponent* entity, Vector2 position);
};
