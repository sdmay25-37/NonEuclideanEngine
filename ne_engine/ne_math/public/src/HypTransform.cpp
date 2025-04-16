#include "HypTransform.hpp"

void HypTransform::to_hyperboloid_ortho(std::vector<glm::vec4>& points)
{
    for(int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = std::sqrt(1.0f + std::pow(x, 2) + std::pow(y, 2));

        points[i] = {x, y, z, 1.0f};
    }
}

void HypTransform::to_plane_ortho(std::vector<glm::vec4>& points)
{
    for(int i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;
        float z = 0.0f;

        points[i] = {x, y, z, 1.0f};
    }
}

void HypTransform::poincare_to_hyperboloid(std::vector<glm::vec4>& points)
{
    for(int i = 0; i < points.size(); i++)
    {
        float factor  = 1 / (1 - std::pow(points[i].x, 2) - std::pow(points[i].y, 2));
        float x = 2 * points[i].x * factor;
        float y = 2 * points[i].y * factor;
        float z = factor * std::sqrt(1 + std::pow(points[i].x, 2) + std::pow(points[i].y, 2));

        points[i] = {x, y, z, 1.0f};
    }
}

// void HypTransform::transform_3d_2d(std::vector<glm::vec4>& points)
// {
//     for(int i = 0; i < points.size(); i++)
//     {
//         points[i].x = points[i].x / points[i].z;/// (points[i].z + 1);
//         points[i].y = points[i].y / points[i].z;/// (points[i].z + 1);
//         points[i].z = -1.0f;
//     }
// }

/* Archive of some other things I have tried*/
// float x = std::sqrt( 1.0f / (std::pow(1/points[i].x, 2) + 1));
// float y = std::sqrt( 1.0f / (std::pow(1/points[i].y, 2) + 1));
// float z = std::sqrt(1.0f + std::pow(x, 2) + std::pow(y, 2));

// float x = points[i].x / (1 + std::sqrt(1 - std::pow(points[i].x, 2) - std::pow(points[i].y, 2)));
// float y = points[i].y / (1 + std::sqrt(1 - std::pow(points[i].x, 2) - std::pow(points[i].y, 2)));
// float z = std::sqrt(1.0f + std::pow(x, 2) + std::pow(y, 2));

// float x = points[i].y * std::exp(points[i].x);
// float y = points[i].y * std::exp(-points[i].x);
// float z = std::sqrt(1.0f + std::pow(x, 2) + std::pow(y, 2));