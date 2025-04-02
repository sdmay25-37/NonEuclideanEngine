#ifndef CIRCLEARCS_HPP
#define CIRCLEARCS_HPP

#include "Polygon.hpp"

class CircleArcs
{
    public:
    CircleArcs(const Polygon& poly);
    ~CircleArcs();

    bool within_circles(const Point& point);

    private:
    void calculate_circle(const Point& point1, const Point& point2);

    std::vector<Point> centers;
    std::vector<float> radii;

};

#endif