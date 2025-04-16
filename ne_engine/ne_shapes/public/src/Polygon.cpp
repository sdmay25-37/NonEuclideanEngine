#include "Polygon.hpp"

Polygon::Polygon(int num_vertices)
    : Polygon(num_vertices, COLOR::RED)
{
    vertices.reserve(num_vertices);
}

Polygon::Polygon(int num_vertices, const Color &color)
{
    vertices.reserve(num_vertices);
    this->color = color;
}

Polygon::~Polygon()
{
}

unsigned int Polygon::vertices_size() const
{
    return vertices.size();
}

const Point *Polygon::vertices_data() const
{
    return vertices.data();
}

unsigned int Polygon::mesh_size() const
{
    return mesh.size();
}

const MeshPoint *Polygon::mesh_data() const
{
    return mesh.data();
}

unsigned int Polygon::indices_size() const
{
    return mesh_indices.size();
}

const unsigned int *Polygon::indices_data() const
{
    return mesh_indices.data();
}

const std::vector<Point> &Polygon::vec() const
{
    return vertices;
}

void Polygon::to_weirstrass()
{
    mesh.to_weirstrass();
}

void Polygon::to_poincare()
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].to_poincare();
    }
}
