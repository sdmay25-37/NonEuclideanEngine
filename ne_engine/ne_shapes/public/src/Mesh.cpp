#include "Mesh.hpp"

Mesh::Mesh(const Color& color)
: color(color)
{

}

void Mesh::gen_poly_mesh()
{

}

void Mesh::rot_x(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_x(theta);
    }
}

void Mesh::rot_y(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_y(theta);
    }
}

void Mesh::rot_z(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_z(theta);
    }
}

void Mesh::translate(float x, float y, float z)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].translate(x, y, z);
    }
}

void Mesh::rot_x_hyp(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_x_hyp(theta);
    }
}

void Mesh::rot_y_hyp(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_y_hyp(theta);
    }
}

void Mesh::rot_z_hyp(float theta)
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_z_hyp(theta);
    }
}

void Mesh::to_weirstrass()
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].to_weirstrass();
    }
}

void Mesh::to_poincare()
{
    for(int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].to_poincare();
    }
}

Point* Mesh::data()
{
    return poly_mesh.data();
}

unsigned int Mesh::data_size()
{
    return poly_mesh.size();
}

void* Mesh::data_offset()
{
    return (void*)((intptr_t)(&(poly_mesh[0].x)) - (intptr_t)(&(poly_mesh[0])));
}

void* Mesh::color_offset()
{
    return (void*)((intptr_t)(&(poly_mesh[0].color)) - (intptr_t)(&(poly_mesh[0])));
}

unsigned int* Mesh::indices_data()
{
    return poly_indices.data();
}

unsigned int Mesh::indices_size()
{
    return poly_indices.size();
}