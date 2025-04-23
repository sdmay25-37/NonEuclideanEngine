#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "Point.hpp"

class Element
{
public:
    Element() = default;
    ~Element() = default;

    virtual Point *data() = 0;
    virtual unsigned int data_size() = 0;
    virtual void *data_offset() = 0;
    virtual void *color_offset() = 0;
    virtual void *uv_offset() = 0;

    virtual unsigned int *indices_data() = 0;
    virtual unsigned int indices_size() = 0;
};

#endif