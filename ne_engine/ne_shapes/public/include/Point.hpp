#ifndef POINT_HPP
#define POINT_HPP

enum class PointType
{
    NONE,
    POINCARE,
    WEIRSTRASS,
    EUCLIDEAN
};

struct Point
{
    float x, y, z, w;
    PointType type;

    Point();
    Point(float x, float y, float z);
    Point(float x, float y, float z, PointType point_type);
    ~Point();

    void to_weirstrass();
    void to_poincare();

    PointType point_type();

    float mag() const;
    float dist(const Point& point) const;
    float dot(const Point& point) const;
    Point cross(const Point& point) const;

    Point operator +(const Point& point) const
    {
        Point p;
        p.x = this->x + point.x;
        p.y = this->y + point.y;
        p.z = this->z + point.z;
        if(this->type == point.type)
        {
            p.type = this->type;
        }

        return p;
    }

    Point operator -(const Point& point) const
    {
        Point p;
        p.x = this->x - point.x;
        p.y = this->y - point.y;
        p.z = this->z - point.z;
        if(this->type == point.type)
        {
            p.type = this->type;
        }

        return p;
    }
};
// static_assert(sizeof(Point) == 20, "Probs won't work");

#endif