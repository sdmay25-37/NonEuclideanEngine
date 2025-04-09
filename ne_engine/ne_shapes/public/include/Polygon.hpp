#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>

#include "Point.hpp"
#include "Mesh.hpp"

class Polygon
{
    public:

    Polygon();
    Polygon(int num_vertices);
    Polygon(int num_vertices, const Color& color);
    ~Polygon();

    void to_weirstrass();
    void to_poincare();

    unsigned int vertices_size() const;
    const Point* vertices_data() const;

    unsigned int mesh_size() const;
    const MeshPoint* mesh_data() const;

    unsigned int indices_size() const;
    const unsigned int* indices_data() const;
    const std::vector<Point>& vec() const;

    protected:
    std::vector<Point> vertices;
    Color color;

    Mesh mesh;
    std::vector<unsigned int> mesh_indices;

};

#endif