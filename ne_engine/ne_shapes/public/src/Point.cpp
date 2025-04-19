#include <cmath>

#include "Point.hpp"

Point::Point()
    : Point(0.0f, 0.0f, 0.0f)
{
}

Point::Point(float x, float y, float z)
    : Point(x, y, z, COLOR::BLACK)
{
}

Point::Point(float x, float y, float z, const Color &color)
    : Point(x, y, z, color, PointType::EUCLIDEAN)
{
}

Point::Point(float x, float y, float z, PointType point_type)
    : Point(x, y, z, COLOR::BLACK, point_type)
{
}
Point::Point(float x, float y, float z, const Color &color, PointType point_type)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = 1.0;
    this->color = color;
    this->type = point_type;
}

Point::~Point()
{
}

void Point::to_weirstrass()
{
    if (type == PointType::POINCARE)
    {
        float denom = (1 - std::pow(x, 2) - std::pow(y, 2));

        z = (1 + std::pow(x, 2) + std::pow(y, 2)) / denom;
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
    if (type == PointType::WEIRSTRASS)
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

float Point::mag() const
{
    float mag = std::sqrt(x * x + y * y + z * z);

    return mag;
}

float Point::dist(const Point &point) const
{
    Point p = *this - point;

    return p.mag();
}

float Point::dot(const Point &point) const
{
    float result = this->x * point.x + this->y * point.y + this->z * point.z;

    return result;
}

Point Point::cross(const Point &point) const
{
    Point p;
    p.x = this->y * point.z - this->z * point.y;
    p.y = this->z * point.x - this->x * point.z;
    p.z = this->x * point.y - this->y * point.x;

    return p;
}

void Point::rotateXHyperbolic(float theta)
{
    float y_new = std::cosh(theta) * this->y + std::sinh(theta) * this->z;
    float z_new = std::sinh(theta) * this->y + std::cosh(theta) * this->z;

    this->y = y_new;
    this->z = z_new;
}
void Point::rotateYHyperbolic(float theta)
{

    float x_new = std::cosh(theta) * x + std::sinh(theta) * z;
    float z_new = std::sinh(theta) * x + std::cosh(theta) * z;

    x = x_new;
    z = z_new;
}