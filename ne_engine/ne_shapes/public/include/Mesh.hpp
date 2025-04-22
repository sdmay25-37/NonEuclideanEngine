#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <cstring>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Point.hpp"
#include "Element.hpp"

class Mesh : public Transformable, Element
{
public:
    Mesh(const Color &color);
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

    Point *data() override;
    unsigned int data_size() override;
    void *data_offset() override;
    void *color_offset() override;

    void rotateXHyperbolic(float theta);
    void rotateYHyperbolic(float theta);

    Mesh &operator=(const Mesh &mesh)
    {
        this->NUM_X_POINTS = mesh.NUM_X_POINTS;
        this->NUM_Y_POINTS = mesh.NUM_Y_POINTS;
        this->MIN_X = mesh.MIN_X;
        this->MAX_X = mesh.MAX_X;
        this->MIN_Y = mesh.MIN_Y;
        this->MAX_Y = mesh.MAX_Y;
        this->color = mesh.color;

        this->gen_mesh();

        return *this;
    }

private:
    void gen_mesh();

    unsigned int NUM_X_POINTS;
    unsigned int NUM_Y_POINTS;
    float MIN_X;
    float MAX_X;
    float MIN_Y;
    float MAX_Y;
    unsigned int *indices_data() override;
    unsigned int indices_size() override;

protected:
    virtual void gen_poly_mesh();

    Color color;

    std::vector<Point> poly_vertices;
    std::vector<Point> poly_mesh;
    std::vector<unsigned int> poly_indices;
};

#endif