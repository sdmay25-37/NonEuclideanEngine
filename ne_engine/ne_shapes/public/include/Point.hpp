#ifndef POINT_HPP
#define POINT_HPP


struct Color
{
    float R;
    float G;
    float B;
    float A;
    Color() = default;
    constexpr Color(float red, float green, float blue, float alpha)
    : R(red), G(green), B(blue), A(alpha)
    {

    }
};

class COLOR
{
    public:
    static constexpr Color RED   = Color(1.0f, 0.0f, 0.0f, 1.0f);
    static constexpr Color BLUE  = Color(0.0f, 0.0f, 1.0f, 1.0f);
    static constexpr Color GREEN = Color(0.0f, 1.0f, 0.0f, 1.0f);
    static constexpr Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
    static constexpr Color WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
};

/* Color Constants Go Here*/
extern Color COLOR_RED  ;
extern Color COLOR_BLUE ;
extern Color COLOR_GREEN;
extern Color COLOR_BLACK;
extern Color COLOR_WHITE;

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
    Color color;
    PointType type;

    Point();
    Point(float x, float y, float z);
    Point(float x, float y, float z, const Color& color);
    Point(float x, float y, float z, PointType point_type);
    Point(float x, float y, float z, const Color& color, PointType point_type);
    ~Point();

    void to_weirstrass();
    void to_poincare();

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