#pragma once
#include "PhysicsComponent.h"
#include <unordered_map>

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
    void Insert(PhysicsComponent* newEntity);
    // Remove an entity -- one moment it's there, the next it's gone
    // Does the same as Insert, but you know... the opposite
    void Remove(PhysicsComponent* entity);
    // Removes and re-inserts an entity
    void Update(PhysicsComponent* changedEntity);
    // Runs a function that performs an interaction between all nearby entites
    void PerformInteractions(void (*functionPtr)(PhysicsComponent*, PhysicsComponent*));
    // Runs a function on every entity in the quadtree
    // This version also allows the function to take in another argument
    template<typename T>
    inline void PerformFunction(void (*functionPtr)(PhysicsComponent*, T), T args)
    {
        for (PhysicsComponent* ent : entities)
        {
            functionPtr(ent, args);
        }
        if (alreadySplit)
        {
            for (int i = 0; i < 4; i++)
            {
                branches[i]->PerformFunction<T>(functionPtr, args);
            }
        }
    }
    void PerformFunction(void (*functionPtr)(PhysicsComponent*));

    // Draw some nice rectangles for debugging purposes
    void DebugDraw();

    size_t TallyEntities();
    std::vector<PhysicsComponent*> GetAllEntities();

    // I don't feel like querying for quads that might have to be updated -- screw it, just update 'em all
    // Should be very cheap (it doesn't do anything!) if it doesn't need to be updated, although ideally this function should be avoided...
    void CheckAndCollapseChildren();

    static constexpr int maxLevel = 5;
private:
    // Used by PerformInteractions
    void PerformChildInteractions(void (*functionPtr)(PhysicsComponent*, PhysicsComponent*), PhysicsComponent* a);
    
    // for managing children
    void Split();

    // For easily (and quickly) finding the quad an entity is in
    static std::unordered_map<PhysicsComponent*, Quadtree*, std::hash<PhysicsComponent*>> quadFinder;
};
