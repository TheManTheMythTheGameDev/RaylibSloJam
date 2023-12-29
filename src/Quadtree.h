#pragma once
#include "Entity.h";

class Quadtree
{
public:
    bool alreadySplit;
    int capacity;
    int level;
    Vector2 position;
    Vector2 dimensions;
    std::vector<PhysicsComponent*> entities;
    Quadtree* nwBranch;
    Quadtree* neBranch;
    Quadtree* swBranch;
    Quadtree* seBranch;
    Quadtree* parent;

    Quadtree(int capacity_ = 1, Vector2 position_ = Vector2{0, 0}, Vector2 dimensions_ = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() }, Quadtree* parent_ = nullptr, int level_ = 0)
    {
        alreadySplit = false;
        capacity = capacity_;
        position = position_;
        dimensions = dimensions_;
        nwBranch = nullptr;
        neBranch = nullptr;
        swBranch = nullptr;
        seBranch = nullptr;
        parent = parent_;
        level = level_;
    }
    ~Quadtree();
    void Split();
    void Insert(PhysicsComponent* newEntity);
    void Remove(PhysicsComponent* entity);
    void Update(PhysicsComponent* changedEntity);

    static constexpr int maxLevel = 5;
private:
    size_t TallyEntities();
    std::vector<PhysicsComponent*> GetAllEntities();
    void IRemove(PhysicsComponent* entity);
    void UpdateAll();
};