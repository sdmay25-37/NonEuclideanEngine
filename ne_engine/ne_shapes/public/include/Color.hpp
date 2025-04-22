#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdexcept>

struct Color
{
    float R;
    float G;
    float B;
    float A;

    Color() = default;
    constexpr Color(float red, float green, float blue, float alpha = 1.0f)
    : R(red), G(green), B(blue), A(alpha)
    {

    }

    float& operator [](unsigned int index) const
    {
        if(index > 3)
        {
            throw std::out_of_range("Index Out of Range for Color");
        }
        float* color = (float*)this;
        return color[index];
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

#endif