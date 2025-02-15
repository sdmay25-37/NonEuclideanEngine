#include "Square.hpp"

Square::Square(square_type type)
{
    points.reserve(4);
    indices = std::vector<unsigned int>({0, 1, 2, 3, 0});

    switch (type)
    {
    case SQUARE_XY:
        points =
        {
            {-.5f,  .5f, 0, 1.0f},
            {-.5f, -.5f, 0, 1.0f},
            { .5f, -.5f, 0, 1.0f},
            { .5f,  .5f, 0, 1.0f}
        };
        break;

    case SQUARE_YZ:
        points =
        {
            {0, -.5f,  .5f, 1.0f},
            {0, -.5f, -.5f, 1.0f},
            {0,  .5f, -.5f, 1.0f},
            {0,  .5f,  .5f, 1.0f}
        };
        break;

    case SQUARE_ZX:
        points =
        {
            { .5f, 0, -.5f, 1.0f},
            {-.5f, 0, -.5f, 1.0f},
            {-.5f, 0,  .5f, 1.0f},
            { .5f, 0,  .5f, 1.0f}
        };
        break;

    default:
        points =
        {
            {0, 0, 0, 1.0f},
            {0, 0, 0, 1.0f},
            {0, 0, 0, 1.0f},
            {0, 0, 0, 1.0f}
        };
        break;
    }
}

Square::~Square()
{

}

std::vector<glm::vec4> Square::getPoints()
{
    return points;
}

std::vector<unsigned int> Square::getIndices()
{
    return indices;
}
