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

    MIN_Y = poly_mesh[0].y;
    MAX_Y = poly_mesh[0].y;
    MIN_X = poly_mesh[0].x;
    MAX_X = poly_mesh[0].x;

    // Loop through all mesh points to find the minimum and maximum y and x values
    for (const auto &point : poly_vertices)
    {
        MIN_Y = std::min(MIN_Y, point.y); // Update MIN_Y if the current point's y is smaller
        MAX_Y = std::max(MAX_Y, point.y); // Update MAX_Y if the current point's y is larger
        MIN_X = std::min(MIN_X, point.x); // Update MIN_X if the current point's x is smaller
        MAX_X = std::max(MAX_X, point.x); // Update MAX_X if the current point's x is larger
    }

    unsigned int point_index = 1;
    for (int i = 0; i < poly_vertices.size(); i++)
    {
        const Point &point1 = poly_vertices[i];
        const Point &point2 = poly_vertices[(i + 1) % poly_vertices.size()];

        // Calculate differences for angle
        Point diff1 = point1 - circle_centers[i];
        Point diff2 = point2 - circle_centers[i];

        const float THETA_START = std::atan2(diff2.y, diff2.x);
        float THETA_END = std::atan2(diff1.y, diff1.x);
        if (THETA_END < THETA_START)
            THETA_END += 2 * M_PI;

        // Difference between angles
        float theta_diff = THETA_END - THETA_START;
        if (theta_diff < 0)
            theta_diff += 2 * M_PI;

        const float THETA_INCR = theta_diff / points_per_arc;

        float theta = THETA_START;
        for (int j = 0; j < points_per_arc - 1; j++)
        {
            Point mesh_point = Point(color, PointType::POINCARE);

            // Calculate x, y coordinates based on theta
            float x = std::cos(theta) * circle_radii[i] + circle_centers[i].x;
            float y = std::sin(theta) * circle_radii[i] + circle_centers[i].y;
            if (y < 0.0 and x < 0.0)
            {
                std::cout << "IM STUPID" << x << "\n"
                          << x - MIN_X << "\n";
                std::cout << "IM STUPID2 " << y << "\n"
                          << y - MIN_Y << "\n";
            }

            // Debug: print out the calculated UV values for checking
            // std::cout << "x: " << x << " y: " << y << " theta: " << theta << std::endl;

            // Adjust uv_u and uv_v based on positions
            float uv_u = (x - MIN_X) / (MAX_X - MIN_X);
            float uv_v = (y - MIN_Y) / (MAX_Y - MIN_Y);

            // Clamp just in case (e.g., due to floating point errors)

            // Optional debug logging
            if (std::abs(x - MIN_X) < 0.001f && std::abs(y - MIN_Y) < 0.001f)
            {
                std::cout << "Bottom-left corner: uv_u: " << uv_u << " uv_v: " << uv_v << std::endl;
                std::cout << "Max X: " << MAX_X << " Min X: " << MIN_X << std::endl;
                std::cout << "Max Y: " << MAX_Y << " Min Y: " << MIN_Y << std::endl;
            }

            if (std::abs(x - MAX_X) < 0.001f && std::abs(y - MAX_Y) < 0.001f)
                std::cout << "Top-right corner: uv_u: " << uv_u << " uv_v: " << uv_v << std::endl;

            if (x == 0.0 and y == 0.0)
            {
                std::cout << "uv_u: " << uv_u << " uv_v: " << uv_v << std::endl;
            }
            if (x < 0)
            {
                // float x_reflected = -y;
                // float y_reflected = -x;

                // uv_u = x + 0.5;
                // uv_u = (x_reflected - MIN_X) / (MAX_X - MIN_X);
                // uv_v = (y_reflected - MIN_Y) / (MAX_Y - MIN_Y);
                // uv_u = (1.0 + x - MIN_X);
                // uv_v = (1.0 + y - MIN_Y);
                // std::cout << "uv_u: " << uv_u << " uv_v: " << uv_v << std::endl;
            }
            // if (x < 0)
            // {
            //     uv_u = 1.0f - uv_u; // Flip the UV horizontally for negative x values
            // }
            // if (y < 0)
            // {
            //     uv_v = 1.0f - uv_v; // Flip the UV horizontally for negative x values
            // }
            // Debug: print out the UVs

            // Set the mesh point with UV coordinates
            mesh_point.x = x;
            mesh_point.y = y;
            mesh_point.uv = glm::vec2(uv_u, uv_v);

            // Store the mesh point and indices
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