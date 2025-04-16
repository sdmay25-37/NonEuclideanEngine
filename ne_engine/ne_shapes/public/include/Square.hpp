#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "Rectangle.hpp"

class Square : public Rectangle
{
    public:
    Square() = default;
    Square(float size);
    Square(float size, unsigned int verts_per_side);
    ~Square();
};
#endif