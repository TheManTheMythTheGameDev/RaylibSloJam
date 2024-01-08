#include "Quadtree.h"
#include "raylib.h"
#include <algorithm>

std::unordered_map<PhysicsComponent*, Quadtree*, std::hash<PhysicsComponent*>> Quadtree::quadFinder;

Quadtree::~Quadtree()
{
    for (int i = 0; i < 4; i++)
    {
        delete branches[i];
        branches[i] = nullptr;
    }
}

void Quadtree::Split()
{
    alreadySplit = true;
    Vector2 newDimensions = Vector2Scale(dimensions, 0.5f);
    branches[NW] = new Quadtree(capacity, position, newDimensions, level + 1);
    branches[NE] = new Quadtree(capacity, Vector2{ position.x + newDimensions.x, position.y }, newDimensions, level + 1);
    branches[SW] = new Quadtree(capacity, Vector2{ position.x, position.y + newDimensions.y }, newDimensions, level + 1);
    branches[SE] = new Quadtree(capacity, Vector2Add(position, newDimensions), newDimensions, level + 1);
    for (int i = entities.size() - 1; i >= 0; i--)
    {
        PhysicsComponent* entity = entities.back();
        entities.pop_back();
        Insert(entity);
    }
}

void Quadtree::Insert(PhysicsComponent* newEntity)
{
     if ((!alreadySplit && entities.size() < capacity) || level >= maxLevel)
    {
        entities.push_back(newEntity);
        quadFinder[newEntity] = this;
    }
    else
    {
        if (!alreadySplit)
        {
            Split();
            Insert(newEntity);
            return;
        }
        else
        {
            Shape entityShape = newEntity->GetShape();
            Rectangle recs[4] =
            {
                Rectangle{ branches[NW]->position.x, branches[NW]->position.y, branches[NW]->dimensions.x, branches[NW]->dimensions.y },
                Rectangle{ branches[NE]->position.x, branches[NE]->position.y, branches[NE]->dimensions.x, branches[NE]->dimensions.y },
                Rectangle{ branches[SW]->position.x, branches[SW]->position.y, branches[SW]->dimensions.x, branches[SW]->dimensions.y },
                Rectangle{ branches[SE]->position.x, branches[SE]->position.y, branches[SE]->dimensions.x, branches[SE]->dimensions.y }
            };
            int recIndex = -1;
            bool multipleChildren = false;
            switch (entityShape.shapeType)
            {
            case (Shape::ShapeType::Circle):
            {
                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionCircleRec(newEntity->position, entityShape.shapeData.circleRadius, recs[i]))
                    {
                        if (recIndex != -1)
                        {
                            multipleChildren = true;
                            break;
                        }
                        recIndex = i;
                    }
                }
                break;
            }
            case (Shape::ShapeType::Rectangle):
            {
                float w = entityShape.shapeData.rectangleData.width;
                float h = entityShape.shapeData.rectangleData.height;
                Rectangle shapeRec = Rectangle{ newEntity->position.x - (w / 2.0f), newEntity->position.y - (h / 2.0f), w, h };
                for (int i = 0; i < 4; i++)
                {
                    if (CheckCollisionRecs(shapeRec, recs[i]))
                    {
                        if (recIndex != -1)
                        {
                            multipleChildren = true;
                            break;
                        }
                        recIndex = i;
                    }
                }
                break;
            }
            default:
                break;
            }
            // If the entity fits within multiple children, it should go in the smallest node that fully contains it
            if (multipleChildren)
            {
                entities.push_back(newEntity);
                quadFinder[newEntity] = this;
            }
            else
            {
                if (recIndex >= 0 && recIndex <= 3)
                {
                    branches[recIndex]->Insert(newEntity);
                }
            }
        }
    }
}

void Quadtree::Remove(PhysicsComponent* entity)
{
    Quadtree* quad = quadFinder[entity];
    quad->entities.erase(std::remove(quad->entities.begin(), quad->entities.end(), entity));
    quadFinder.erase(entity);
}

void Quadtree::Update(PhysicsComponent* changedEntity)
{
    Remove(changedEntity);
    Insert(changedEntity);
}

void Quadtree::PerformInteractions(void(*functionPtr)(PhysicsComponent*, PhysicsComponent*))
{
    for (int i = 0; i < (int)(entities.size()) - 1; i++)
    {
        for (int j = i + 1; j < (int)(entities.size()); j++)
        {
            functionPtr(entities[i], entities[j]);
        }
    }
    if (alreadySplit)
    {
        for (int i = 0; i < entities.size(); i++)
        {
            for (int c = 0; c < 4; c++)
            {
                branches[c]->PerformChildInteractions(functionPtr, entities[i]);
            }
        }
        for (int i = 0; i < 4; i++)
        {
            branches[i]->PerformInteractions(functionPtr);
        }
    }
}

void Quadtree::PerformFunction(void(*functionPtr)(PhysicsComponent*))
{
    for (PhysicsComponent* ent : entities)
    {
        functionPtr(ent);
    }
    if (alreadySplit)
    {
        for (int i = 0; i < 4; i++)
        {
            branches[i]->PerformFunction(functionPtr);
        }
    }
}

size_t Quadtree::TallyEntities()
{
    int sum = entities.size();
    if (alreadySplit)
    {
        for (int i = 0; i < 4; i++)
        {
            sum += branches[i]->TallyEntities();
        }
    }
    return sum;
}

std::vector<PhysicsComponent*> Quadtree::GetAllEntities()
{
    std::vector<PhysicsComponent*> result = std::vector<PhysicsComponent*>(TallyEntities());
    int index = 0;
    std::copy(entities.begin(), entities.end(), result.begin() + index);
    index += entities.size();
    // In this case, iteration is better than recursion. Recursion would result in far more dynamic memory allocation
    // Recursion:
    if (alreadySplit)
    {
        for (int i = 0; i < 4; i++)
        {
            std::vector<PhysicsComponent*> branchComps = branches[i]->GetAllEntities();
            std::copy(branchComps.begin(), branchComps.end(), result.begin() + index);
            index += branchComps.size();
        }
    }
    return result;
}

void Quadtree::CheckAndCollapseChildren()
{
    int branchSum = 0;
    if (alreadySplit)
    {
        // All children should be collapsed first
        for (int i = 0; i < 4; i++)
        {
            branches[i]->CheckAndCollapseChildren();
        }
        for (int i = 0; i < 4; i++)
        {
            branchSum += branches[i]->entities.size();
        }
    }
    else
    {
        branchSum = -1; // Not 0
    }
    if (TallyEntities() <= capacity || branchSum == 0)
    {
        std::vector<PhysicsComponent*> allEntities = GetAllEntities();
        entities = allEntities;
        for (int i = 0; i < 4; i++)
        {
            delete branches[i];
            branches[i] = nullptr;
        }
        for (PhysicsComponent* comp : allEntities)
        {
            quadFinder[comp] = this;
        }
        alreadySplit = false;
    }
}

void Quadtree::PerformChildInteractions(void(*functionPtr)(PhysicsComponent*, PhysicsComponent*), PhysicsComponent* a)
{
    for (PhysicsComponent* b : entities)
    {
        functionPtr(a, b);
    }
    if (alreadySplit)
    {
        for (int i = 0; i < 4; i++)
        {
            branches[i]->PerformChildInteractions(functionPtr, a);
        }
    }
}

void Quadtree::DebugDraw()
{
    DrawText(TextFormat("%i", entities.size()), position.x + 10 + (level * 10), position.y + 10, 10, BLACK);
    for (PhysicsComponent* comp : entities)
    {
        // DrawCircle(comp->position.x, comp->position.y, 50.0f, Color{ 0, 0, 255, 30 });
    }
    if (alreadySplit)
    {
        for (int i = 0; i < 4; i++)
        {
            branches[i]->DebugDraw();
        }
    }
    else
    {
        DrawRectangleLines(position.x, position.y, dimensions.x, dimensions.y, RED);
    }
}
