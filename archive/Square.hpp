#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <vector>
#include <glm/glm.hpp>

class Square
{
    public:

    typedef enum
    {
        SQUARE_XY,
        SQUARE_YZ,
        SQUARE_ZX,
        SQUARE_EMPTY
    }square_type;

    Square(square_type type);

    ~Square();

    std::vector<glm::vec4> getPoints();
    std::vector<unsigned int> getIndices();

    Square operator*(float scalar)
    {
        Square square = Square(SQUARE_EMPTY);
        for(int i = 0; i < points.size();i++)
        {
            square.points[i] = points[i] * scalar;
        }

        return square;
    }

    Square operator*(glm::mat4 transform)
    {
        Square square = Square(SQUARE_EMPTY);
        for(int i = 0; i < points.size();i++)
        {
            square.points[i] = transform * points[i];
        }

        return square;
    }

    private:

    std::vector<glm::vec4> points;
    std::vector<unsigned int> indices;

};


#endif