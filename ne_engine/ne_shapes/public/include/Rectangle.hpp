#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <vector>
#include "glm/glm.hpp"

#define DEFAULT_VERTS_PER_SIDE 2

class Rectangle
{
    public:
    Rectangle() = default;
    Rectangle(float height, float width);
    Rectangle(float height, float width, unsigned int num_x_verts, unsigned int num_y_verts);
    ~Rectangle();

    void to_3d_hyperbolic();
    void offset(const glm::vec3& offset);

    std::vector<glm::vec4>& get_verts();
    std::vector<unsigned int>& get_indices();

    protected:
    Rectangle(float height, float width, unsigned int num_x_verts, unsigned int num_y_verts, bool gen_verts);
    std::vector<glm::vec4> vertices;
    std::vector<unsigned int> indices;
    float height;
    float width;
    unsigned int x_verts;
    unsigned int y_verts;

    virtual void genVerts();
    void genIndices();

};
#endif