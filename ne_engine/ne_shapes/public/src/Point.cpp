#include <cmath>

#include "Point.hpp"

Point::Point(float x, float y, float z, const Color &color, const PointType &point_type)
{
    this->x = x;
    this->y = y;
    this->z = z;

    this->w = 1.0;
    this->color = color;
    this->type = point_type;

    zero_under_threshold();
}
Point::Point(const Color &color, const PointType &point_type)
    : Point(0.0f, 0.0f, 0.0f, color, point_type)
{
}

Point::~Point()
{
}

void Point::rot_x(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x;
    float y_new = y * std::cos(theta) - z * std::sin(theta);
    float z_new = y * std::sin(theta) + z * std::cos(theta);

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
}

void Point::rot_y(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x * std::cos(theta) - z * std::sin(theta);
    float y_new = y;
    float z_new = x * std::sin(theta) + z * std::cos(theta);

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
}

void Point::rot_z(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x * std::cos(theta) - y * std::sin(theta);
    ;
    float y_new = x * std::sin(theta) + y * std::cos(theta);
    float z_new = z;

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
}

void Point::translate(float x, float y, float z)
{
    Point t = Point(x, y, z);

    *this += t;

    zero_under_threshold();
}

void Point::rot_x_hyp(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x;
    float y_new = y * std::cosh(theta) + z * std::sinh(theta);
    float z_new = y * std::sinh(theta) + z * std::cosh(theta);

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
}

void Point::rot_y_hyp(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x * std::cosh(theta) + z * std::sinh(theta);
    float y_new = y;
    float z_new = x * std::sinh(theta) + z * std::cosh(theta);

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
}

void Point::rot_z_hyp(float theta)
{
    theta = angle_unit_conv(theta);
    float x_new = x * std::cosh(theta) + y * std::sinh(theta);
    ;
    float y_new = x * std::sinh(theta) + y * std::cosh(theta);
    float z_new = z;

    x = x_new;
    y = y_new;
    z = z_new;

    zero_under_threshold();
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

    zero_under_threshold();
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

    p.zero_under_threshold();

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
#define ZERO_THRESHOLD 1e-6f
void Point::zero_under_threshold()
{
    if (std::abs(x) < ZERO_THRESHOLD)
    {
        x = 0.0f;
    }
    if (std::abs(y) < ZERO_THRESHOLD)
    {
        y = 0.0f;
    }
    if (std::abs(z) < ZERO_THRESHOLD)
    {
        z = 0.0f;
    }
}