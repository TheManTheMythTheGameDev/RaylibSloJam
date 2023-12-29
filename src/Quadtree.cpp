#include "Quadtree.h"
#include "raylib.h"
#include <algorithm>

Quadtree::~Quadtree()
{
    for (int i = 0; i < 4; i++)
    {
        delete branches[i];
    }
}

void Quadtree::Split()
{
    alreadySplit = true;
    Vector2 newDimensions = Vector2Scale(dimensions, 0.5f);
    branches[NW] = new Quadtree(capacity, position, newDimensions, this, level + 1);
    branches[NE] = new Quadtree(capacity, Vector2{ position.x + newDimensions.x, position.y }, newDimensions, this, level + 1);
    branches[SW] = new Quadtree(capacity, Vector2{ position.x, position.y + newDimensions.y }, newDimensions, this, level + 1);
    branches[SE] = new Quadtree(capacity, Vector2Add(position, newDimensions), newDimensions, this, level + 1);
    for (PhysicsComponent* entity : entities)
    {
        this->Insert(entity);
    }
    entities.clear();
}

void Quadtree::Insert(PhysicsComponent* newEntity)
{
    if ((!alreadySplit && entities.size() < capacity) || level >= maxLevel)
    {
        entities.push_back(newEntity);
    }
    else
    {
        if (!alreadySplit)
        {
            Split();
            this->Insert(newEntity);
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
            }
            else
            {
                branches[recIndex]->Insert(newEntity);
            }
        }
    }
}

void Quadtree::Remove(PhysicsComponent* entity)
{
    IRemove(entity);
}

void Quadtree::Update(PhysicsComponent* changedEntity, Vector2 oldPosition)
{

}

size_t Quadtree::TallyEntities()
{
    if (alreadySplit)
    {
        return branches[NW]->TallyEntities() + branches[NE]->TallyEntities() + branches[SW]->TallyEntities() + branches[SE]->TallyEntities();
    }
    else
    {
        return entities.size();
    }
}

std::vector<PhysicsComponent*> Quadtree::GetAllEntities()
{
    std::vector<PhysicsComponent*> result;
    if (alreadySplit)
    {
        std::vector<PhysicsComponent*> nwComps = branches[NW]->GetAllEntities();
        result.insert(result.end(), nwComps.begin(), nwComps.end());
        std::vector<PhysicsComponent*> neComps = branches[NE]->GetAllEntities();
        result.insert(result.end(), neComps.begin(), neComps.end());
        std::vector<PhysicsComponent*> swComps = branches[SW]->GetAllEntities();
        result.insert(result.end(), swComps.begin(), swComps.end());
        std::vector<PhysicsComponent*> seComps = branches[SE]->GetAllEntities();
        result.insert(result.end(), seComps.begin(), seComps.end());
    }
    else
    {
        result.insert(result.end(), entities.begin(), entities.end());
    }
    return result;
}

void Quadtree::IRemove(PhysicsComponent* entity)
{
    Shape entityShape = entity->GetShape();
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
            if (CheckCollisionCircleRec(entity->position, entityShape.shapeData.circleRadius, recs[i]))
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
        Rectangle shapeRec = Rectangle{ entity->position.x - (w / 2.0f), entity->position.y - (h / 2.0f), w, h };
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
        entities.erase(std::remove(entities.begin(), entities.end(), entity));
    }
    else
    {
        branches[recIndex]->IRemove(entity);
    }
}

void Quadtree::UpdateAll()
{
    if (alreadySplit)
    {
        if (TallyEntities() <= capacity)
        {
            std::vector<PhysicsComponent*> allEntities = GetAllEntities();
            entities = allEntities;
            for (int i = 0; i < 4; i++)
            {
                delete branches[i];
            }
        }
        else
        {
            for (int i = 0; i < 4; i++)
            {
                branches[i]->UpdateAll();
            }
        }
    }
    else
    {
        if (!alreadySplit && entities.size() > capacity)
        {
            Split();
        }
    }
}

void Quadtree::MoveEntity(PhysicsComponent* currentData, Vector2 oldPosition, Quadtree** oldSlot, Quadtree** newSlot)
{
}
