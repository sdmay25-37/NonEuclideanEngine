#include "Rectangle.hpp"
#include "HypTransform.hpp"
#include <iostream>

Rectangle::Rectangle(float height, float width)
: Rectangle(height, width, DEFAULT_VERTS_PER_SIDE, DEFAULT_VERTS_PER_SIDE, true)
{
}

Rectangle::Rectangle(float height, float width, unsigned int num_x_verts, unsigned int num_y_verts)
: Rectangle(height, width, num_x_verts, num_y_verts, true)
{
}

Rectangle::Rectangle(float height, float width, unsigned int num_x_verts, unsigned int num_y_verts, bool gen_verts)
{
    x_verts = num_x_verts;
    y_verts = num_y_verts;

    this->height = height;
    this->width = width;

    if(x_verts < 2) x_verts = 2;
    if(y_verts < 2) y_verts = 2;

    genIndices();

    if(gen_verts)
    {
        genVerts();
    }

}

Rectangle::~Rectangle()
{

}

void Rectangle::to_3d_hyperbolic()
{
    HypTransform::poincare_to_hyperboloid(vertices);
}

void Rectangle::offset(const glm::vec3& offset)
{
    for(unsigned int i =0; i < vertices.size();i++)
    {
        vertices[i][0] += offset[0];
        vertices[i][1] += offset[1];
        vertices[i][2] += offset[2];
    }
}

std::vector<glm::vec4>& Rectangle::get_verts()
{
    return vertices;
}

std::vector<unsigned int>& Rectangle::get_indices()
{
    return indices;
}

void Rectangle::genVerts()
{
    const float START_X = -width/2.0f;
    const float END_X = width/2.0f;
    const float INCR_X = width/x_verts;

    const float START_Y = -height/2.0f;
    const float END_Y = height/2.0f;
    const float INCR_Y = height/y_verts;

    float x = 0;
    float y = 0;

    for(unsigned int i = 0; i < x_verts; i++)
    {
        if(i == (x_verts - 1)) x = END_X;
        else x = START_X + INCR_X * i;

        for(unsigned int j = 0; j < y_verts;j++)
        {
            if(j == (y_verts - 1)) y = END_Y;
            else y = START_Y + INCR_Y * j;

            vertices.push_back({x, y, 0, 1});
        }
    }
}

void Rectangle::genIndices()
{
    for(unsigned int i = 0; i < x_verts - 1; i++)
    {
        for(unsigned int j = 0; j < y_verts - 1; j++)
        {
            unsigned int p0_index = j + x_verts * i;
            unsigned int p1_index = j + x_verts * (i + 1);
            unsigned int p2_index = j + 1 + x_verts * i;
            unsigned int p3_index = j + 1 + (x_verts * (i + 1));

            indices.push_back(p0_index);
            indices.push_back(p1_index);
            indices.push_back(p2_index);

            indices.push_back(p1_index);
            indices.push_back(p2_index);
            indices.push_back(p3_index);
        }
    }
}