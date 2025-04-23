#ifndef HYPMESH_HPP
#define HYPMESH_HPP

#include "Mesh.hpp"

#define DEFAULT_ARC_POINTS 500

class HypMesh : public Mesh
{
public:
    HypMesh(const Color &color, unsigned int points_per_arc = DEFAULT_ARC_POINTS);
    ~HypMesh();

protected:
    void gen_poly_mesh() override;

private:
    void gen_circles();
    void init_poly_mesh();

    std::vector<Point> circle_centers;
    std::vector<float> circle_radii;

    unsigned int points_per_arc;
    unsigned int mesh_size;
};

#endif