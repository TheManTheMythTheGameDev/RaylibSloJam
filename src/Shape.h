#pragma once

class Shape
{
public:
    enum class ShapeType
    {
        Circle = 0,
        Rectangle
    };

    struct RectangleDimensions
    {
        float width, height;
    };

    union ShapeData
    {
        float circleRadius;
        RectangleDimensions rectangleData;
    };

    Shape(ShapeType _shapeType = ShapeType::Circle, ShapeData _shapeData = ShapeData{ 100.0f })
    {
        shapeType = _shapeType;
        shapeData = _shapeData;
    }

    ShapeType shapeType;
    ShapeData shapeData;
};
