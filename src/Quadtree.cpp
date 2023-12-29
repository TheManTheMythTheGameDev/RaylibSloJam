#include "Quadtree.h"
#include "raylib.h"
#include <algorithm>

Quadtree::~Quadtree()
{
    delete nwBranch;
    delete neBranch;
    delete swBranch;
    delete seBranch;
}

void Quadtree::Split()
{
    alreadySplit = true;
    Vector2 newDimensions = Vector2Scale(dimensions, 0.5f);
    nwBranch = new Quadtree(capacity, position, newDimensions, this, level + 1);
    neBranch = new Quadtree(capacity, Vector2{ position.x + newDimensions.x, position.y }, newDimensions, this, level + 1);
    swBranch = new Quadtree(capacity, Vector2{ position.x, position.y + newDimensions.y }, newDimensions, this, level + 1);
    seBranch = new Quadtree(capacity, Vector2Add(position, newDimensions), newDimensions, this, level + 1);
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

void Quadtree::Remove(PhysicsComponent* entity)
{
    IRemove(entity);
}

void Quadtree::Update(PhysicsComponent* changedEntity)
{
}

size_t Quadtree::TallyEntities()
{
    if (alreadySplit)
    {
        return nwBranch->TallyEntities() + neBranch->TallyEntities() + swBranch->TallyEntities() + seBranch->TallyEntities();
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
        std::vector<PhysicsComponent*> nwComps = nwBranch->GetAllEntities();
        result.insert(result.end(), nwComps.begin(), nwComps.end());
        std::vector<PhysicsComponent*> neComps = neBranch->GetAllEntities();
        result.insert(result.end(), neComps.begin(), neComps.end());
        std::vector<PhysicsComponent*> swComps = swBranch->GetAllEntities();
        result.insert(result.end(), swComps.begin(), swComps.end());
        std::vector<PhysicsComponent*> seComps = seBranch->GetAllEntities();
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
    if (alreadySplit)
    {
        Shape entityShape = entity->GetShape();
        Rectangle nwRec, neRec, swRec, seRec;
        nwRec = Rectangle{ nwBranch->position.x, nwBranch->position.y, nwBranch->dimensions.x, nwBranch->dimensions.y };
        neRec = Rectangle{ neBranch->position.x, neBranch->position.y, neBranch->dimensions.x, neBranch->dimensions.y };
        swRec = Rectangle{ swBranch->position.x, swBranch->position.y, swBranch->dimensions.x, swBranch->dimensions.y };
        seRec = Rectangle{ seBranch->position.x, seBranch->position.y, seBranch->dimensions.x, seBranch->dimensions.y };
        switch (entityShape.shapeType)
        {
        case (Shape::ShapeType::Circle):
        {
            if (CheckCollisionCircleRec(entity->position, entityShape.shapeData.circleRadius, nwRec))
            {
                nwBranch->Remove(entity);
            }
            if (CheckCollisionCircleRec(entity->position, entityShape.shapeData.circleRadius, neRec))
            {
                neBranch->Remove(entity);
            }
            if (CheckCollisionCircleRec(entity->position, entityShape.shapeData.circleRadius, swRec))
            {
                swBranch->Remove(entity);
            }
            if (CheckCollisionCircleRec(entity->position, entityShape.shapeData.circleRadius, seRec))
            {
                seBranch->Remove(entity);
            }
            break;
        }
        case (Shape::ShapeType::Rectangle):
        {
            float w = entityShape.shapeData.rectangleData.width;
            float h = entityShape.shapeData.rectangleData.height;
            Rectangle shapeRec = Rectangle{ entity->position.x - (w / 2.0f), entity->position.y - (h / 2.0f), w, h };
            if (CheckCollisionRecs(shapeRec, nwRec))
            {
                nwBranch->Remove(entity);
            }
            if (CheckCollisionRecs(shapeRec, neRec))
            {
                neBranch->Remove(entity);
            }
            if (CheckCollisionRecs(shapeRec, swRec))
            {
                swBranch->Remove(entity);
            }
            if (CheckCollisionRecs(shapeRec, seRec))
            {
                seBranch->Remove(entity);
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        entities.erase(std::remove(entities.begin(), entities.end(), entity));
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
            delete nwBranch;
            delete neBranch;
            delete swBranch;
            delete seBranch;
        }
        else
        {
            nwBranch->UpdateAll();
            neBranch->UpdateAll();
            swBranch->UpdateAll();
            seBranch->UpdateAll();
        }
    }
    else
    {
        if (entities.size() > capacity)
        {
            Split();
        }
    }
}
