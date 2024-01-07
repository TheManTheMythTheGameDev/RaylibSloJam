#include "DestructibleWallComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include <random>

DestructibleWallComponent::DestructibleWallComponent(Scene& _scene, Texture2D _circleTex, Vector2 _dims)
    : scene(_scene), circleTex(_circleTex), dims(_dims)
{
}

DestructibleWallComponent::~DestructibleWallComponent()
{
}

void DestructibleWallComponent::Update(Entity& parentEntity)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        int numCircles = GetRandomValue(minCircles, maxCircles);
        Circle* circles = PackCircles(numCircles, parentEntity.pos);

        for (int i = 0; i < numCircles; i++)
        {
            Circle circle = circles[i];
            Entity ent = Entity(circle.pos, 0.0f, 0, new GraphicsComponent(circleTex, circle.radius, circle.radius));
            scene.DeferAddEntity(ent);
        }

        delete[] circles;

        scene.DeferDeleteEntity(parentEntity);
    }
}

DestructibleWallComponent::Circle* DestructibleWallComponent::PackCircles(int numCircles, Vector2 pos)
{
    Circle* circles = new Circle[numCircles];

    Vector2 topLeft = Vector2Subtract(pos, Vector2Scale(dims, 0.5f));
    Vector2 bottomRight = Vector2Add(topLeft, dims);

    float rectangleArea = dims.x * dims.y;
    // Rearranging A = n * pi * r^2 to solve for r
    float approximateRadius = sqrtf(rectangleArea / (numCircles * PI));
    float minSize = approximateRadius - 5.0f;
    float maxSize = approximateRadius + 10.0f;

    for (int i = 0; i < numCircles; i++)
    {
        Circle cur = Circle{ 0 };
        float size = GetRandomValue(minSize * 100, maxSize * 100) / 100.0f;
        cur.radius = size;
        if (i == 0)
        {
            cur.pos = Vector2Add(topLeft, Vector2{ cur.radius, cur.radius });
        }
        else
        {
            Vector2 prospectivePos = Vector2Add(circles[i - 1].pos, Vector2{ cur.radius + circles[i - 1].radius, 0 });
            if (prospectivePos.x > pos.x + dims.x / 2 + cur.radius)
            {
                cur.pos.x = topLeft.x + cur.radius;
                cur.pos.y = topLeft.y;
            }
            else
            {
                cur.pos = prospectivePos;
            }
            // Find the lowest possible Y available
            for (int j = 0; j < i; j++)
            {
                Circle otherCircle = circles[j];
                if (Vector2Distance(cur.pos, otherCircle.pos) < cur.radius + otherCircle.radius - 0.1f)
                {
                    cur.pos.y = otherCircle.pos.y + (otherCircle.radius + cur.radius);
                }
            }
        }
        circles[i] = cur;
    }

    return circles;
}
