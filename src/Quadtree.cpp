#include "Quadtree.h"
#include "raylib.h"

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
            // If an entity spans multiple quads, it should be added to each of them
            Shape entityShape = newEntity->GetShape();
            Rectangle nwRec, neRec, swRec, seRec;
            nwRec = Rectangle{ nwBranch->position.x, nwBranch->position.y, nwBranch->dimensions.x, nwBranch->dimensions.y };
            neRec = Rectangle{ neBranch->position.x, neBranch->position.y, neBranch->dimensions.x, neBranch->dimensions.y };
            swRec = Rectangle{ swBranch->position.x, swBranch->position.y, swBranch->dimensions.x, swBranch->dimensions.y };
            seRec = Rectangle{ seBranch->position.x, seBranch->position.y, seBranch->dimensions.x, seBranch->dimensions.y };
            switch (entityShape.shapeType)
            {
            case (Shape::ShapeType::Circle):
            {
                if (CheckCollisionCircleRec(newEntity->position, entityShape.shapeData.circleRadius, nwRec))
                {
                    nwBranch->Insert(newEntity);
                }
                if (CheckCollisionCircleRec(newEntity->position, entityShape.shapeData.circleRadius, neRec))
                {
                    neBranch->Insert(newEntity);
                }
                if (CheckCollisionCircleRec(newEntity->position, entityShape.shapeData.circleRadius, swRec))
                {
                    swBranch->Insert(newEntity);
                }
                if (CheckCollisionCircleRec(newEntity->position, entityShape.shapeData.circleRadius, seRec))
                {
                    seBranch->Insert(newEntity);
                }
                break;
            }
            case (Shape::ShapeType::Rectangle):
            {
                float w = entityShape.shapeData.rectangleData.width;
                float h = entityShape.shapeData.rectangleData.height;
                Rectangle shapeRec = Rectangle{ newEntity->position.x - (w / 2.0f), newEntity->position.y - (h / 2.0f), w, h };
                if (CheckCollisionRecs(shapeRec, nwRec))
                {
                    nwBranch->Insert(newEntity);
                }
                if (CheckCollisionRecs(shapeRec, neRec))
                {
                    neBranch->Insert(newEntity);
                }
                if (CheckCollisionRecs(shapeRec, swRec))
                {
                    swBranch->Insert(newEntity);
                }
                if (CheckCollisionRecs(shapeRec, seRec))
                {
                    seBranch->Insert(newEntity);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}