#include "Square.hpp"

Square::Square(float size)
:Rectangle(size, size)
{

}

Square::Square(float size, unsigned int verts_per_side)
:Rectangle(size, size, verts_per_side, verts_per_side)
{

}

Square::~Square()
{

}