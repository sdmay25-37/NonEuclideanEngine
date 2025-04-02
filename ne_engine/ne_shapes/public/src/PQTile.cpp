#include <cmath>
#include <stdexcept>

#include "PQTile.hpp"
#include "CircleArcs.hpp"

PQTile::PQTile(int p, int q)
: Polygon(p)
{
    float tile_check = (p - 2) * (q - 2);

    if(tile_check <= 4)
    {
        throw std::invalid_argument("(p - 2) * (q - 2) Must be greater then 4");
    }

    this->p = p;
    this->q = q;

    gen_vertices();
    get_indices();
}

PQTile::~PQTile()
{

}

void PQTile::gen_vertices()
{
    // Initialize tile as the center polygon in an n-k regular or quasiregular tiling.
    // Let ABC be a triangle in a regular (n,k0-tiling, where
    //    A is the center of an n-gon (also center of the disk),
    //    B is a vertex of the n-gon, and
    //    C is the midpoint of a side of the n-gon adjacent to B.
    float angleA = M_PI / p;
    float angleB = M_PI / q;
    float angleC = M_PI / 2.0;

    // For a regular tiling, we need to compute the distance s from A to B.
    float sinA = std::sin(angleA);
    float sinB = std::sin(angleB);

    // Calculate the distance and normalize it
    float d = std::sin(angleC - angleB - angleA)
            / std::sqrt(1.0 - sinB * sinB - sinA * sinA);

    for(int i = 0; i < p; i++)
    {
        float x = d * std::cos((1 + 2 * i) * angleA);
        float y = d * std::sin((1 + 2 * i) * angleA);
        float z = 0;

        vertices.emplace_back(Point(x, y, z, PointType::POINCARE));
    }
}

void PQTile::get_indices()
{
    CircleArcs circleArcs = CircleArcs(*this);
    mesh = Mesh(vertices);

    for(int i = 0; i < mesh.size();i++)
    {
        MeshPoint* up = mesh[i].up;
        MeshPoint* right = mesh[i].right;

        if(up == nullptr || right == nullptr)
        {
            continue;
        }

        MeshPoint* up_right = up->right;

        if(up_right == nullptr || circleArcs.within_circles(mesh[i]) || circleArcs.within_circles(*up_right))
        {
            continue;
        }

        bool test = (!circleArcs.within_circles(*up));
        // First build up-right triangle
        if((up != nullptr) && (!circleArcs.within_circles(*up)))
        {
            mesh_indices.emplace_back(i);
            mesh_indices.emplace_back(up->index);
            mesh_indices.emplace_back(up_right->index);
        }

        // Then right-up triangle
        if((right != nullptr) && !(circleArcs.within_circles(*right)))
        {
            mesh_indices.emplace_back(i);
            mesh_indices.emplace_back(right->index);
            mesh_indices.emplace_back(up_right->index);
        }
    }
}