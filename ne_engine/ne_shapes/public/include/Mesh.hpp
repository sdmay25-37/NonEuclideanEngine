#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "Point.hpp"
#include "Element.hpp"

class Mesh : public Transformable, Element
{
    public:
    Mesh(const Color& color);
    ~Mesh() = default;

    void rot_x(float theta) override;
    void rot_y(float theta) override;
    void rot_z(float theta) override;
    void translate(float x, float y, float z) override;

    void rot_x_hyp(float theta) override;
    void rot_y_hyp(float theta) override;
    void rot_z_hyp(float theta) override;

    void to_weirstrass() override;
    void to_poincare() override;

    Point* data() override;
    unsigned int data_size() override;
    void* data_offset() override;
    void* color_offset() override;

    unsigned int* indices_data() override;
    unsigned int indices_size() override;

    protected:
    virtual void gen_poly_mesh();

    Color color;

    std::vector<Point> poly_vertices;
    std::vector<Point> poly_mesh;
    std::vector<unsigned int> poly_indices;
};

#endif