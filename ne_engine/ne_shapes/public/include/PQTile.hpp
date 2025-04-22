#ifndef PQTILE_HPP
#define PQTILE_HPP

#include "HypMesh.hpp"

class PQTile : public HypMesh
{
    public:
    PQTile(int p=4, int q=5, const Color& color=COLOR::RED);
    ~PQTile();

    private:

    void gen_vertices();
    void gen_indices();

    int p;
    int q;

};
#endif