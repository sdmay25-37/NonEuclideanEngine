#ifndef POINT_HPP
#define POINT_HPP

#include <glm/glm.hpp>

#include "Color.hpp"
#include "Transformable.hpp"

enum class PointType
{
    NONE,
    POINCARE,
    WEIRSTRASS,
    EUCLIDEAN
};

struct Point : public Transformable
{
    float x, y, z, w;
    Color color;
    PointType type;

    Point(float x, float y, float z, const Color &color, PointType point_type);
    Point(float x = 0.0f, float y = 0.0f, float z = 0.0f, const Color &color = COLOR::RED, const PointType &point_type = PointType::NONE);
    ~Point();

    void rot_x(float theta) override;
    void rot_y(float theta) override;
    void rot_z(float theta) override;
    void translate(float x, float y, float z) override;

    void rot_x_hyp(float theta) override;
    void rot_y_hyp(float theta) override;
    void rot_z_hyp(float theta) override;

    void to_weirstrass() override;
    void to_poincare() override;

    void rotateXHyperbolic(float theta);
    void rotateYHyperbolic(float theta);
    float mag() const;
    float dist(const Point &point) const;
    float dot(const Point &point) const;
    Point cross(const Point &point) const;

    Point operator+(const Point &point) const
    {
        Point p;
        p.x = this->x + point.x;
        p.y = this->y + point.y;
        p.z = this->z + point.z;
        p.color = this->color;

        if (this->type == point.type)
        {
            p.type = this->type;
        }

        p.zero_under_threshold();
        return p;
    }

    Point &operator+=(const Point &point)
    {
        this->x += point.x;
        this->y += point.y;
        this->z += point.z;

        this->zero_under_threshold();
        return *this;
    }

    Point operator-(const Point &point) const
    {
        Point p;
        p.x = this->x - point.x;
        p.y = this->y - point.y;
        p.z = this->z - point.z;
        p.color = this->color;
        if (this->type == point.type)
        {
            p.type = this->type;
        }

        p.zero_under_threshold();
        return p;
    }

    Point &operator-=(const Point &point)
    {
        this->x += point.x;
        this->y += point.y;
        this->z += point.z;

        this->zero_under_threshold();

        return *this;
    }

    Point operator*(float mul) const
    {
        Point p;
        p.x = this->x * mul;
        p.y = this->y * mul;
        p.z = this->z * mul;
        p.color = this->color;
        p.type = this->type;

        p.zero_under_threshold();

        return p;
    }

    Point &operator*=(float div)
    {
        this->x *= div;
        this->y *= div;
        this->z *= div;

        this->zero_under_threshold();
        return *this;
    }

    Point operator/(float div) const
    {
        Point p;
        p.x = this->x / div;
        p.y = this->y / div;
        p.z = this->z / div;
        p.color = this->color;
        p.type = this->type;

        p.zero_under_threshold();

        return p;
    }

    Point &operator/=(float div)
    {
        this->x /= div;
        this->y /= div;
        this->z /= div;

        this->zero_under_threshold();

        return *this;
    }

    float &operator[](unsigned int index) const
    {
        if (index > 3)
        {
            throw std::out_of_range("Index Out of Range for Color");
        }
        float *coord = (float *)this;
        return coord[index];
    }

private:
    void zero_under_threshold();
};

#endif