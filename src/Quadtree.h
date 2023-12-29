#pragma once
#include "Entity.h";

class Quadtree
{
public:
    bool alreadySplit;
    int capacity;
    Vector2 position;
    Vector2 dimensions;
    std::vector<PhysicsComponent*> entities;
    Quadtree* nwBranch;
    Quadtree* neBranch;
    Quadtree* swBranch;
    Quadtree* seBranch;

    Quadtree(int capacity_ = 1, Vector2 position_ = Vector2{0, 0}, Vector2 dimensions_ = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() })
    {
        alreadySplit = false;
        capacity = capacity_;
        position = position_;
        dimensions = dimensions_;
        nwBranch = nullptr;
        neBranch = nullptr;
        swBranch = nullptr;
        seBranch = nullptr;
    }
    void Split();
    void Insert(PhysicsComponent* newEntity);
};