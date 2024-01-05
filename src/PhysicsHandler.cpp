#include "PhysicsHandler.h"
#include <chrono>

void PhysicsHandler::CalculateSteps(float dTime) {
    currentTime += dTime;
    while (currentTime > stepTime) {
        Step();
        currentTime -= stepTime;
    }
}

void PhysicsHandler::Step() {
    auto beginTime = std::chrono::high_resolution_clock::now();

    // To avoid calling GetAllEntities, which does a lot of dynamic memory allocation, let's set oldPosition this way instead
    spatialPartition.PerformFunction([](PhysicsComponent* ent)
        {
            ent->oldPosition = ent->position;
        });
    // Equivalent to:
    /*std::vector<PhysicsComponent*> allEntities = spatialPartition.GetAllEntities();
    for (PhysicsComponent* physicsEnt : allEntities)
    {
        physicsEnt->oldPosition = physicsEnt->position;
    }*/
    spatialPartition.CheckAndCollapseChildren();
    spatialPartition.PerformInteractions(PhysicsInteraction);

    // Again, let's avoid GetAllEntities and dynamic memory allocation
    spatialPartition.PerformFunction([](PhysicsComponent* physicsEnt)
        {
            if (physicsEnt->mobile)
            {
                physicsEnt->velocity = Vector2Add(physicsEnt->velocity, Vector2Scale(physicsEnt->acceleration, stepTime));
                physicsEnt->position = Vector2Add(physicsEnt->position, Vector2Scale(physicsEnt->velocity, stepTime));
            }
            if (physicsEnt->position.x < -borderLength)
            {
                physicsEnt->position.x += GetScreenWidth() + 2 * borderLength;
            }
            if (physicsEnt->position.x > GetScreenWidth() + borderLength)
            {
                physicsEnt->position.x -= GetScreenWidth() + 2 * borderLength;
            }
            if (physicsEnt->position.y < -borderLength)
            {
                physicsEnt->position.y += GetScreenHeight() + 2 * borderLength;
            }
            if (physicsEnt->position.y > GetScreenHeight() + borderLength)
            {
                physicsEnt->position.y -= GetScreenHeight() + 2 * borderLength;
            }
        });
    spatialPartition.PerformFunction<Quadtree&>([](PhysicsComponent* physicsEnt, Quadtree& quad)
        {
            if (physicsEnt->oldPosition.x != physicsEnt->position.x || physicsEnt->oldPosition.y != physicsEnt->position.y)
            {
                quad.Update(physicsEnt);
            }
        }, spatialPartition);
    /*allEntities = spatialPartition.GetAllEntities();
    for (PhysicsComponent* physicsEnt : allEntities) 
    {
        if (physicsEnt->mobile)
        {
            physicsEnt->velocity = Vector2Add(physicsEnt->velocity, Vector2Scale(physicsEnt->acceleration, stepTime));
            physicsEnt->position = Vector2Add(physicsEnt->position, Vector2Scale(physicsEnt->velocity, stepTime));
        }
        if (physicsEnt->position.x < -borderLength) 
        {
            physicsEnt->position.x += GetScreenWidth() + 2 * borderLength;
        }
        if (physicsEnt->position.x > GetScreenWidth() + borderLength) 
        {
            physicsEnt->position.x -= GetScreenWidth() + 2 * borderLength;
        }
        if (physicsEnt->position.y < -borderLength) 
        {
            physicsEnt->position.y += GetScreenHeight() + 2 * borderLength;
        }
        if (physicsEnt->position.y > GetScreenHeight() + borderLength) 
        {
            physicsEnt->position.y -= GetScreenHeight() + 2 * borderLength;
        }
        if (physicsEnt->oldPosition.x != physicsEnt->position.x || physicsEnt->oldPosition.y != physicsEnt->position.y)
        {
            spatialPartition.Update(physicsEnt);
        }
    }*/
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - beginTime);
    // std::cout << "Physics time: " << elapsed.count() << " microseconds (" << elapsed.count() / 100rectRotation << ") ms\n";
}

bool PhysicsHandler::pointInBox(Vector2 pointPos, Vector2 rectPos, float width, float height, float rotation)
{
    Vector2 rectToPoint = Vector2Subtract(pointPos, rectPos);
    float currentRotation = atanf(rectToPoint.y / rectToPoint.x) - rotation;
    if (rectToPoint.y < 0)
    {
        currentRotation += PI;
    }
    rectToPoint = Vector2Scale({ cosf(currentRotation), sinf(currentRotation) }, Vector2Length(rectToPoint));
    return rectToPoint.x < rectPos.x + width / 2 && rectToPoint.x > rectPos.x - width / 2 && rectToPoint.y < rectPos.y + height / 2 && rectToPoint.y > rectPos.y - height / 2;
}

float PhysicsHandler::DistanceToLineSegment(Vector2 point, Vector2 corner1, Vector2 corner2)
{
    float lineLengthSqr = Vector2DistanceSqr(corner2, corner1);
    if (lineLengthSqr == 0)
    {
        return Vector2Distance(corner1, point);
    }
    float t = Clamp(Vector2DotProduct(Vector2Subtract(point, corner1), Vector2Subtract(corner2, corner1)) / lineLengthSqr, 0, 1);
    Vector2 pivotPoint = Vector2Add(corner1, Vector2Scale(Vector2Subtract(corner2, corner1), t));
    DrawCircle(pivotPoint.x, pivotPoint.y, 5, PURPLE);
    return Vector2DistanceSqr(point, Vector2Add(corner1, Vector2Scale(Vector2Subtract(corner2, corner1), t)));
}

void PhysicsHandler::Bounce(PhysicsComponent* mobileEntity, Vector2 normalUnitVector)
{
    Vector2 normalVelocity = Vector2Scale(normalUnitVector, Vector2DotProduct(mobileEntity->velocity, normalUnitVector));
    Vector2 tangentVelocity = Vector2Subtract(mobileEntity->velocity, normalVelocity);
    mobileEntity->velocity = Vector2Add(Vector2Scale(normalVelocity, -1), tangentVelocity);
}

void PhysicsHandler::ResolveCircleCollision(PhysicsComponent* entity1, PhysicsComponent* entity2)
{
    if (!entity1->mobile || !entity2->mobile)
    {
        PhysicsComponent* mobileEntity;
        PhysicsComponent* staticEntity;
        if (entity1->mobile)
        {
            mobileEntity = entity1;
            staticEntity = entity2;
        }
        else
        {
            mobileEntity = entity2;
            staticEntity = entity1;
        }
        Vector2 normalUnitVector = Vector2Normalize(Vector2Subtract(staticEntity->position, mobileEntity->position));
        Bounce(mobileEntity, normalUnitVector);
        float overlapDist = (mobileEntity->GetShape().shapeData.circleRadius + staticEntity->GetShape().shapeData.circleRadius - Vector2Distance(mobileEntity->position, staticEntity->position));
        mobileEntity->position = Vector2Subtract(mobileEntity->position, Vector2Scale(normalUnitVector, overlapDist));
    }
    else
    {
        Vector2 normalUnitVector = Vector2Normalize(Vector2Subtract(entity2->position, entity1->position));
        Vector2 entity1NormalVelocity = Vector2Scale(normalUnitVector, Vector2DotProduct(entity1->velocity, normalUnitVector));
        Vector2 entity1TangentVelocity = Vector2Subtract(entity1->velocity, entity1NormalVelocity);
        Vector2 negativeNormalVector = Vector2Scale(normalUnitVector, -1);
        Vector2 entity2NormalVelocity = Vector2Scale(negativeNormalVector, Vector2DotProduct(entity2->velocity, negativeNormalVector));
        Vector2 entity2TangentVelocity = Vector2Subtract(entity2->velocity, entity2NormalVelocity);
        float combinedMass = entity1->mass + entity2->mass;
        Vector2 entity1CollisionVelocity = Vector2Scale(Vector2Add(Vector2Scale(entity1NormalVelocity, entity1->mass - entity2->mass), Vector2Scale(entity2NormalVelocity, 2 * entity2->mass)), 1 / (combinedMass));
        Vector2 entity2CollisionVelocity = Vector2Scale(Vector2Add(Vector2Scale(entity2NormalVelocity, entity2->mass - entity1->mass), Vector2Scale(entity1NormalVelocity, 2 * entity1->mass)), 1 / (combinedMass));
        entity1->velocity = Vector2Add(entity1CollisionVelocity, entity1TangentVelocity);
        entity2->velocity = Vector2Add(entity2CollisionVelocity, entity2TangentVelocity);
        float overlapDist = (entity1->GetShape().shapeData.circleRadius + entity2->GetShape().shapeData.circleRadius - Vector2Distance(entity1->position, entity2->position)) / 2.0;
        entity1->position = Vector2Subtract(entity1->position, Vector2Scale(normalUnitVector, overlapDist));
        entity2->position = Vector2Subtract(entity2->position, Vector2Scale(negativeNormalVector, overlapDist));
    }
    entity1->currentCollided.push_back(entity2->holdingEntity);
    entity2->currentCollided.push_back(entity1->holdingEntity);
}



void PhysicsHandler::AddEntity(PhysicsComponent* newEntity) {
    spatialPartition.Insert(newEntity);
}

void PhysicsHandler::RemoveEntity(PhysicsComponent* entity)
{
    spatialPartition.Remove(entity);
}

void PhysicsHandler::ClearEntities() {
    // TODO: implement
}

void PhysicsHandler::DebugDraw()
{
    spatialPartition.DebugDraw();
}

void PhysicsHandler::PhysicsInteraction(PhysicsComponent* entity1, PhysicsComponent* entity2)
{
    if (entity1->GetShape().shapeType == Shape::ShapeType::Circle && entity2->GetShape().shapeType == Shape::ShapeType::Circle)
    {
        if (entity1->GetShape().shapeData.circleRadius + entity2->GetShape().shapeData.circleRadius > Vector2Distance(entity1->position, entity2->position))
        {
            ResolveCircleCollision(entity1, entity2);
        }
    }
    else
    {
        PhysicsComponent* rectEntity;
        PhysicsComponent* circleEntity;
        if (entity1->GetShape().shapeType == Shape::ShapeType::Rectangle)
        {
            rectEntity = entity1;
            circleEntity = entity2;
        }
        else
        {
            rectEntity = entity2;
            circleEntity = entity1;
        }
        float radius = circleEntity->GetShape().shapeData.circleRadius;
        float width = rectEntity->GetShape().shapeData.rectangleData.width;
        float height = rectEntity->GetShape().shapeData.rectangleData.width;
        float rectRotation = rectEntity->rotation;
        Vector2 circlePos = circleEntity->position;
        Vector2 rectPos = rectEntity->position;
        // float horizWallSlope = tanf(rectRotation);
        // float vertWallSlope = tanf(PI / 2);
        // float horizWallOffset = -horizWallSlope * rectPos.x + rectPos.y;
        // float vertWallOffset = -vertWallSlope * rectPos.x + rectPos.y;
        // float circleVertOffset = abs(circlePos.y - (vertWallSlope * circlePos.x + vertWallOffset)) / sqrt(pow(vertWallOffset, 2) + 1);
        // float circleHorizOffset = abs(circlePos.y - (horizWallSlope * circlePos.x + horizWallOffset)) / sqrt(pow(horizWallOffset, 2) + 1);
        bool intersecting = false;
        // if (circleVertOffset < width / 2 && circleVertOffset < height / 2)
        // {
        //     circlePos = Vector2Subtract(circlePos, circleEntity->velocity);
        //     circleVertOffset = abs(circlePos.y - (vertWallSlope * circlePos.x + vertWallOffset)) / sqrt(pow(vertWallOffset, 2) + 1);
        //     circleHorizOffset = abs(circlePos.y - (horizWallSlope * circlePos.x + horizWallOffset)) / sqrt(pow(horizWallOffset, 2) + 1);
        //     intersecting = true;
        // }
        if (pointInBox(circlePos, rectPos, width, height, rectRotation))
        {
            intersecting = true;
        }
        else
        {
            Vector2 corners[4];
            corners[0] = Vector2{ rectPos.x + (width * cosf(rectRotation) / 2) - (height * sinf(rectRotation) / 2), rectPos.y + (width * sinf(rectRotation) / 2) + (height * cosf(rectRotation) / 2) };
            corners[1] = Vector2{ rectPos.x - (width * cosf(rectRotation) / 2) - (height * sinf(rectRotation) / 2), rectPos.y - (width * sinf(rectRotation) / 2) + (height * cosf(rectRotation) / 2) };
            corners[2] = Vector2Add(rectPos, Vector2Subtract(rectPos, corners[0]));
            corners[3] = Vector2Add(rectPos, Vector2Subtract(rectPos, corners[1]));
            for (int i = 0; i < 4; i++)
            {
                // if(i == 0)
                // {
                //     DrawLine(200, 0, 200, 200, PURPLE);
                //     DistanceToLineSegment(circlePos, { 200, 0 }, { 200, 200 });
                // }
                if (DistanceToLineSegment(circlePos, corners[i], corners[(i + 1) % 4]) < radius * radius)
                {
                    intersecting = true;
                    break;
                }
            }
        }
        if (intersecting)
        {
            Vector2 collisionBoxCorners[4];
            float collisionBoxWidth = width + 2 * radius;
            float collisionBoxHeight = height + 2 * radius;
            collisionBoxCorners[0] = Vector2{ rectPos.x + (collisionBoxWidth * cosf(rectRotation) / 2) - (collisionBoxHeight * sinf(rectRotation) / 2), rectPos.y + (collisionBoxWidth * sinf(rectRotation) / 2) + (collisionBoxHeight * cosf(rectRotation) / 2) };
            collisionBoxCorners[1] = Vector2{ rectPos.x - (collisionBoxWidth * cosf(rectRotation) / 2) - (collisionBoxHeight * sinf(rectRotation) / 2), rectPos.y - (collisionBoxWidth * sinf(rectRotation) / 2) + (collisionBoxHeight * cosf(rectRotation) / 2) };
            collisionBoxCorners[2] = Vector2Add(rectPos, Vector2Subtract(rectPos, collisionBoxCorners[0]));
            collisionBoxCorners[3] = Vector2Add(rectPos, Vector2Subtract(rectPos, collisionBoxCorners[1]));
            Vector2 rayStart;
            Vector2 rayDirection;
            Vector2 oldPos = Vector2Subtract(circlePos, circleEntity->velocity);
            if (pointInBox(circlePos, rectPos, collisionBoxWidth, collisionBoxHeight, rectRotation))
            {
                rayStart = circlePos;
                rayDirection = Vector2Subtract(circlePos, oldPos);
            }
            else
            {
                rayStart = oldPos;
                rayDirection = Vector2Subtract(oldPos, circlePos);
            }
            int bestIntersectionCorner = -1;
            float shortestIntersection = rectRotation;
            Vector2 v3 = { -rayDirection.y, rayDirection.x };
            for (int i = 0; i < 4; i++)
            {
                Vector2 v1 = Vector2Subtract(rayStart, collisionBoxCorners[i]);
                Vector2 v2 = Vector2Subtract(collisionBoxCorners[(i + 1) % 4], collisionBoxCorners[i]);
                float rayIntersectTime = abs(v2.x * v1.y - v2.y * v1.x) / Vector2DotProduct(v2, v3);
                float segmentIntersectTime = Vector2DotProduct(v1, v3) / Vector2DotProduct(v2, v3);
                if (rayIntersectTime > 0 && segmentIntersectTime > 0 && segmentIntersectTime < 1)
                {
                    float intersectDist = Vector2LengthSqr(Vector2Scale(rayDirection, rayIntersectTime));
                    if (bestIntersectionCorner == -1)
                    {
                        bestIntersectionCorner = i;
                        shortestIntersection = intersectDist;
                    }
                    else if (intersectDist < shortestIntersection)
                    {
                        bestIntersectionCorner = i;
                        shortestIntersection = intersectDist;
                    }
                }
            }
            std::cout << bestIntersectionCorner << std::endl;
            Vector2 corner1 = collisionBoxCorners[bestIntersectionCorner];
            Vector2 corner2 = collisionBoxCorners[(bestIntersectionCorner + 1) % 4];
            Vector2 bounceWall = Vector2Normalize(Vector2Subtract(corner2, corner1));
            std::cout << bounceWall.x << " " << bounceWall.y << std::endl;
            Bounce(circleEntity, { -bounceWall.y, bounceWall.x });
            float lineLengthSqr = Vector2DistanceSqr(corner2, corner1);
            float t = Vector2DotProduct(Vector2Subtract(circlePos, corner1), Vector2Subtract(corner2, corner1)) / lineLengthSqr;
            circleEntity->position = Vector2Add(corner1, Vector2Scale(Vector2Subtract(corner2, corner1), t));
        }
        // bool interesctingVert = circleVertOffset < radius + width / 2;
        // bool interesctingHoriz = circleVertOffset < radius + height / 2;
        // bool interesecting = interesctingVert && interesctingHoriz;

        // if (circleVertOffset < vertIntersectionDist || circleHorizOffset < horizIntersectionDist)
        // {
        //     if (circleVertOffset < width / 2 && circleVertOffset < height / 2)
        //     {
        //         circlePos = Vector2Subtract(circlePos, circleEntity->velocity);
        //         circleVertOffset = abs(circlePos.y - (vertWallSlope * circlePos.x + vertWallOffset)) / sqrt(pow(vertWallOffset, 2) + 1);
        //         circleHorizOffset = abs(circlePos.y - (horizWallSlope * circlePos.x + horizWallOffset)) / sqrt(pow(horizWallOffset, 2) + 1);
        //     }
            
        // }
    }
}
