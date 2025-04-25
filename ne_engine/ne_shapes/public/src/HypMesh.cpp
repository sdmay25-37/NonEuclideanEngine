#include <cmath>
#include <stdexcept>
#include <iostream>
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

    std::vector<Point> arc_points;
    arc_points.reserve(poly_vertices.size() * points_per_arc);

    unsigned int point_index = 1;

    for (int i = 0; i < poly_vertices.size(); i++)
    {
        const Point &point1 = poly_vertices[i];
        const Point &point2 = poly_vertices[(i + 1) % poly_vertices.size()];

        Point diff1 = point1 - circle_centers[i];
        Point diff2 = point2 - circle_centers[i];

        float THETA_START = std::atan2(diff2.y, diff2.x);
        float THETA_END = std::atan2(diff1.y, diff1.x);
        if (THETA_END < THETA_START)
            THETA_END += 2 * M_PI;

        float theta_diff = THETA_END - THETA_START;
        float THETA_INCR = theta_diff / points_per_arc;

        float theta = THETA_START;

        for (int j = 0; j < points_per_arc; j++)
        {
            Point mesh_point = Point(color, PointType::POINCARE);
            float x = std::cos(theta) * circle_radii[i] + circle_centers[i].x;
            float y = std::sin(theta) * circle_radii[i] + circle_centers[i].y;

            mesh_point.x = x;
            mesh_point.y = y;

            arc_points.emplace_back(mesh_point);

            // Set up indices immediately
            poly_indices.emplace_back(CENTER_INDEX);
            poly_indices.emplace_back(point_index);
            poly_indices.emplace_back((point_index + 1) % mesh_size == 0 ? 1 : (point_index + 1));
            point_index++;

            theta += THETA_INCR;
        }
    }

    // ==== SECOND PASS: Find bounds for real arc mesh ====
    MIN_X = arc_points[0].x;
    MAX_X = arc_points[0].x;
    MIN_Y = arc_points[0].y;
    MAX_Y = arc_points[0].y;

    for (const auto &pt : arc_points)
    {
        MIN_X = std::min(MIN_X, pt.x);
        MAX_X = std::max(MAX_X, pt.x);
        MIN_Y = std::min(MIN_Y, pt.y);
        MAX_Y = std::max(MAX_Y, pt.y);
    }

    // ==== FINAL PASS: Assign UVs ====
    for (auto &pt : arc_points)
    {
        float uv_u = (pt.x - MIN_X) / (MAX_X - MIN_X);
        float uv_v = (pt.y - MIN_Y) / (MAX_Y - MIN_Y);
        pt.uv = glm::vec2(uv_u, uv_v);
        poly_mesh.emplace_back(pt);
    }
}

void HypMesh::init_poly_mesh()
{
    // reserve space for all edge points and center point
    mesh_size = (poly_vertices.size() * (points_per_arc)) + 1;
    poly_mesh.reserve(mesh_size);

    Point poly_center = Point(color, PointType::POINCARE);

    for (int i = 0; i < poly_vertices.size(); i++)
    {

        poly_center += poly_vertices[i];
    }
    // poly_center.uv = glm::vec2({(MIN_X + MAX_X) / 2, (MIN_Y + MAX_Y) / 2});
    poly_center.uv = glm::vec2({0.5f, 0.5f});
    poly_center /= poly_vertices.size();

    poly_mesh.emplace_back(poly_center);
}