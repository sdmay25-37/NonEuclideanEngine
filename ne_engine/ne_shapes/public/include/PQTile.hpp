#ifndef PQTILE_HPP
#define PQ_TILE_HPP

#include "Polygon.hpp"

class PQTile : public Polygon
{
    public:
    PQTile() = default;
    PQTile(int p, int q);
    ~PQTile();

    private:

    void gen_vertices();
    void get_indices();

    int p;
    int q;

};
#endif