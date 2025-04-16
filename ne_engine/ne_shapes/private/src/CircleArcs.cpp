#include <cmath>

#include "CircleArcs.hpp"


CircleArcs::CircleArcs(const Polygon& poly)
{
    centers.reserve(poly.vertices_size());
    radii.reserve(poly.vertices_size());
    for(int i = 0; i < poly.vertices_size() - 1;i++)
    {
        calculate_circle(poly.vertices_data()[i], poly.vertices_data()[i+1]);
    }
    calculate_circle(poly.vertices_data()[poly.vertices_size() - 1], poly.vertices_data()[0]);
}

CircleArcs::~CircleArcs()
{

}

bool CircleArcs::within_circles(const Point& point)
{
    for(int i = 0; i < centers.size(); i++)
    {
        if(point.dist(centers[i]) < radii[i])
        {
            return true;
        }
    }

    return false;
}

void CircleArcs::calculate_circle(const Point& point1, const Point& point2)
{
    Point p_cross = point1.cross(point2);

    float den = p_cross.z;

    float s1 = (1.0f + point1.x * point1.x + point1.y * point1.y) / 2.0f;
    float s2 = (1.0f + point2.x * point2.x + point2.y * point2.y) / 2.0f;

    float x = (s1 * point2.y - s2 * point1.y) / den;
    float y = (s2 * point1.x - s1 * point2.x) / den;

    Point c = Point(x, y, 0.0, PointType::POINCARE);
    float radius = std::sqrt(c.x*c.x + c.y*c.y - 1.0f);

    centers.emplace_back(c);
    radii.emplace_back(radius);
}