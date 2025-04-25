#include "Mesh.hpp"
#include <glm/vec2.hpp> // Ensure GLM's vec2 is available
#include <cassert>
#include <iostream>

Mesh::Mesh(const Color &color)
    : color(color)
{
}

void Mesh::gen_poly_mesh()
{
}

void Mesh::rot_x(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_x(theta);
    }
}

void Mesh::rot_y(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_y(theta);
    }
}

void Mesh::rot_z(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_z(theta);
    }
}

void Mesh::translate(float x, float y, float z)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].translate(x, y, z);
    }
}

void Mesh::rot_x_hyp(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_x_hyp(theta);
    }
}

void Mesh::rot_y_hyp(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_y_hyp(theta);
    }
}

void Mesh::rot_z_hyp(float theta)
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rot_z_hyp(theta);
    }
}

void Mesh::to_weirstrass()
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].to_weirstrass();
    }
}

void Mesh::rotateXHyperbolic(float theta)
{
    for (unsigned int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rotateXHyperbolic(theta);
    }
    // recalculate_uvs();
}
void Mesh::recalculate_uvs()
{
    std::vector<glm::vec2> projected_points;
    projected_points.reserve(poly_mesh.size());

    // Project the points after rotation to 2D space (Weierstrass -> Poincaré projection)
    for (const auto &pt : poly_mesh)
    {
        // Apply Poincaré projection to each point after rotation
        float denom = 1.0f + pt.z; // Weierstrass -> Poincaré projection
        projected_points.emplace_back(glm::vec2(pt.x / denom, pt.y / denom));
    }

    // Compute bounds in projected space (min, max in 2D)
    float min_x = projected_points[0].x;
    float max_x = projected_points[0].x;
    float min_y = projected_points[0].y;
    float max_y = projected_points[0].y;

    for (const auto &p : projected_points)
    {
        min_x = std::min(min_x, p.x);
        max_x = std::max(max_x, p.x);
        min_y = std::min(min_y, p.y);
        max_y = std::max(max_y, p.y);
    }

    // Normalize UVs based on the min/max bounds in 2D space
    for (size_t i = 0; i < poly_mesh.size(); ++i)
    {
        float u = (projected_points[i].x - min_x) / (max_x - min_x);
        float v = (projected_points[i].y - min_y) / (max_y - min_y);
        poly_mesh[i].uv = glm::vec2(u, v);
    }

    // Center point gets UV (0.5, 0.5)
    if (!poly_mesh.empty())
        poly_mesh[0].uv = glm::vec2(0.5f, 0.5f);
}

void Mesh::rotateXYHyperbolic(float thetaX, float thetaY)
{
    // Apply X rotation
    if (thetaX != 0.0)
    {
        for (unsigned int i = 0; i < poly_mesh.size(); i++)
        {
            poly_mesh[i].rotateXHyperbolic(thetaX);
        }
    }

    // Apply Y rotation
    if (thetaY != 0.0)
    {
        for (unsigned int i = 0; i < poly_mesh.size(); i++)
        {
            poly_mesh[i].rotateYHyperbolic(thetaY);
        }
    }

    // Recalculate UVs after both rotations
    recalculate_uvs();
}

void Mesh::rotateYHyperbolic(float theta)
{
    for (unsigned int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].rotateYHyperbolic(theta);
    }
    // recalculate_uvs();
}

void Mesh::to_poincare()
{
    for (int i = 0; i < poly_mesh.size(); i++)
    {
        poly_mesh[i].to_poincare();
    }
}

Point *Mesh::data()
{
    return poly_mesh.data();
}

unsigned int Mesh::data_size()
{
    return poly_mesh.size();
}

void *Mesh::data_offset()
{
    return (void *)((intptr_t)(&(poly_mesh[0].x)) - (intptr_t)(&(poly_mesh[0])));
}

void *Mesh::color_offset()
{
    return (void *)((intptr_t)(&(poly_mesh[0].color)) - (intptr_t)(&(poly_mesh[0])));
}

void *Mesh::uv_offset()
{
    return (void *)((intptr_t)(&(poly_mesh[0].uv)) - (intptr_t)(&(poly_mesh[0])));
}

unsigned int *Mesh::indices_data()
{
    return poly_indices.data();
}

unsigned int Mesh::indices_size()
{
    return poly_indices.size();
}
