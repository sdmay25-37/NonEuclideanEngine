#include <cmath>
#include <stdexcept>

#include "HypMesh.hpp"

#define CENTER_INDEX 0
// #define DEFAULT_ARC_POINTS 500

HypMesh::HypMesh(const Color &color, unsigned int points_per_arc)
    : Mesh(color), points_per_arc(points_per_arc)
{
}

HypMesh::~HypMesh()
{
}

void HypMesh::gen_circles()
{
    circle_centers.reserve(poly_vertices.size());
    circle_radii.reserve(poly_vertices.size());

    for (int i = 0; i < poly_vertices.size(); i++)
    {
        const Point &point1 = poly_vertices[i];
        const Point &point2 = poly_vertices[(i + 1) % poly_vertices.size()];

        Point p_cross = point1.cross(point2);

        float den = p_cross.z;

        float s1 = (1.0f + point1.x * point1.x + point1.y * point1.y) / 2.0f;
        float s2 = (1.0f + point2.x * point2.x + point2.y * point2.y) / 2.0f;

        float x = (s1 * point2.y - s2 * point1.y) / den;
        float y = (s2 * point1.x - s1 * point2.x) / den;

        Point c = Point(x, y, 0.0);
        float radius = std::sqrt(c.x * c.x + c.y * c.y - 1.0f);

        circle_centers.emplace_back(c);
        circle_radii.emplace_back(radius);
    }
}

void HypMesh::gen_poly_mesh()
{
    gen_circles();
    init_poly_mesh();

    unsigned int point_index = 1;
    for (int i = 0; i < poly_vertices.size(); i++)
    {
        const Point &point1 = poly_vertices[i];
        const Point &point2 = poly_vertices[(i + 1) % poly_vertices.size()];

        Point diff1 = point1 - circle_centers[i];
        Point diff2 = point2 - circle_centers[i];

        const float THETA_START = std::atan2(diff2.y, diff2.x);
        float THETA_END = std::atan2(diff1.y, diff1.x);
        if (THETA_END < THETA_START)
            THETA_END += 2 * M_PI;

        const float THETA_INCR = (THETA_END - THETA_START) / points_per_arc;

        float theta = THETA_START;

        for (int j = 0; j < points_per_arc - 1; j++)
        {
            Point mesh_point = Point(color, PointType::POINCARE);

            if (j == 0)
            {
                mesh_point = poly_vertices[(i + 1) % poly_vertices.size()];
            }
            else
            {
                float x = std::cos(theta) * circle_radii[i] + circle_centers[i].x;
                float y = std::sin(theta) * circle_radii[i] + circle_centers[i].y;

                mesh_point.x = x;
                mesh_point.y = y;
            }

            poly_mesh.emplace_back(mesh_point);
            poly_indices.emplace_back(CENTER_INDEX);
            poly_indices.emplace_back(point_index);
            if (point_index == (mesh_size - 1))
            {
                poly_indices.emplace_back(1);
            }
            else
            {
                poly_indices.emplace_back((point_index + 1));
            }

            theta += THETA_INCR;
            point_index++;
        }
    }
}

void HypMesh::init_poly_mesh()
{
    // reserve space for all edge points and center point
    mesh_size = (poly_vertices.size() * (points_per_arc - 1)) + 1;
    poly_mesh.reserve(mesh_size);

    Point poly_center = Point(color, PointType::POINCARE);

    for (int i = 0; i < poly_vertices.size(); i++)
    {
        poly_center += poly_vertices[i];
    }

    poly_center /= poly_vertices.size();

    poly_mesh.emplace_back(poly_center);
}