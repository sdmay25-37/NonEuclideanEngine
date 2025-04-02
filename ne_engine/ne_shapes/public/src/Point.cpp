#include <cmath>

#include "Point.hpp"

Point::Point()
: Point(0.0f, 0.0f, 0.0f, PointType::NONE)
{
}

Point::Point(float x, float y, float z)
: Point(x, y, z, PointType::EUCLIDEAN)
{
}

Point::Point(float x, float y, float z, PointType point_type)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1.0;
    this->type = point_type;
}

Point::~Point()
{

}

void Point::to_weirstrass()
{
    if(type == PointType::POINCARE)
    {
        float denom = (1 - std::pow(x, 2) - std::pow(y, 2));

        z = std::sqrt(1 + std::pow(x, 2) + std::pow(y, 2)) / denom;
        x = (2 * x) / denom;
        y = (2 * y) / denom;

        type = PointType::WEIRSTRASS;
    }
    else
    {
        type = PointType::WEIRSTRASS;
    }
}

void Point::to_poincare()
{
    if(type == PointType::WEIRSTRASS)
    {
        x = x / (1 + z);
        y = y / (1 + z);
        z = 0;

        type = PointType::POINCARE;
    }
    else
    {
        type = PointType::POINCARE;
    }
}

PointType Point::point_type()
{
    return type;
}

float Point::mag() const
{
    float mag = std::sqrt(x*x + y*y + z*z);

    return mag;
}

float Point::dist(const Point& point) const
{
    Point p = *this - point;

    return p.mag();
}

float Point::dot(const Point& point) const
{
    float result = this->x * point.x + this->y *point.y + this->z * point.z;

    return result;
}

Point Point::cross(const Point& point) const
{
    Point p;
    p.x = this->y * point.z - this->z * point.y;
    p.y = this->z * point.x - this->x * point.z;
    p.z = this->x * point.y - this->y * point.x;

    return p;
}