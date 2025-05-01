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
    Mesh() = default;
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
    void *uv_offset() override;
    void scale(float factor);
    void scale(float sx, float sy, float sz);

    void rotateXHyperbolic(float theta);
    void rotateYHyperbolic(float theta);
    void rotateXYHyperbolic(float thetaX, float thetaY);
    void recalculate_uvs();

    unsigned int *indices_data() override;
    unsigned int indices_size() override;
    std::vector<Point> poly_vertices;
    std::vector<Point> poly_mesh;

private:
    unsigned int NUM_X_POINTS;
    unsigned int NUM_Y_POINTS;

protected:
    virtual void gen_poly_mesh();
    float MIN_X;
    float MAX_X;
    float MIN_Y;
    float MAX_Y;

    std::vector<unsigned int> poly_indices;
};

#endif