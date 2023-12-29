#include "Quadtree.h"

void Quadtree::Split()
{
    alreadySplit = true;
    Vector2 newDimensions = Vector2Scale(dimensions, 0.5f);
    nwBranch = new Quadtree(capacity, position, newDimensions);
    neBranch = new Quadtree(capacity, Vector2{ position.x + newDimensions.x, position.y }, newDimensions);
    swBranch = new Quadtree(capacity, Vector2{ position.x, position.y + newDimensions.y }, newDimensions);
    seBranch = new Quadtree(capacity, Vector2Add(position, newDimensions), newDimensions);
}

void Quadtree::Insert(PhysicsComponent* newEntity)
{
    if (!alreadySplit && entities.size() < capacity)
    {
        entities.push_back(newEntity);
    }
    else
    {
        if (!alreadySplit)
        {
            Split();
            for (PhysicsComponent* entity : entities)
            {
                this->Insert(entity);
            }
            entities.clear();
        }
        else
        {
            if (newEntity->position.x < position.x + dimensions.x / 2)
            {
                if (newEntity->position.y < position.y + dimensions.y / 2)
                {
                    nwBranch->Insert(newEntity);
                }
                else
                {
                    swBranch->Insert(newEntity);
                }
            }
            else
            {
                if (newEntity->position.y < position.y + dimensions.y / 2)
                {
                    neBranch->Insert(newEntity);
                }
                else
                {
                    seBranch->Insert(newEntity);
                }
            }
        }
    }
}