#ifndef PQTILE_HPP
#define PQ_TILE_HPP

#include "Polygon.hpp"

class PQTile : public Polygon
{
public:
    PQTile() = default;
    PQTile(int p, int q);
    PQTile(int p, int q, const Color &color);
    ~PQTile();

private:
    void gen_vertices();
    void gen_indices();

    int p;
    int q;
};
#endif