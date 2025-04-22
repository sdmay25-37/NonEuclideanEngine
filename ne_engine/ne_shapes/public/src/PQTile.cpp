#include <cmath>
#include <stdexcept>

#include "PQTile.hpp"

PQTile::PQTile(int p, int q)
    : PQTile(p, q, COLOR::RED)
{
}

PQTile::PQTile(int p, int q, const Color &color)
    : HypMesh(color)
{

    float tile_check = (p - 2) * (q - 2);

    if (tile_check <= 4)
    {
        throw std::invalid_argument("(p - 2) * (q - 2) Must be greater then 4");
    }

    this->p = p;
    this->q = q;

    gen_vertices();
    gen_poly_mesh();
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
    float d = std::sin(angleC - angleB - angleA) / std::sqrt(1.0 - sinB * sinB - sinA * sinA);

    for (int i = 0; i < p; i++)
    {
        // Start in lower left corner for square
        float x = d * std::cos((5 + 2 * i) * angleA);
        float y = d * std::sin((5 + 2 * i) * angleA);
        float z = 0.0f;
        poly_vertices.emplace_back(Point(x, y, z, color, PointType::POINCARE));
    }
}
