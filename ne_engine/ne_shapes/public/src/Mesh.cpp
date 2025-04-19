#include "Mesh.hpp"
#include <glm/vec2.hpp> // Ensure GLM's vec2 is available
#include <cassert>

#define DEFAULT_POINTS 250

// Mesh::Mesh()
// : Mesh(DEFAULT_POINTS, DEFAULT_POINTS)
// {
//
// }

Mesh::Mesh(const unsigned int num_x_points, const unsigned int num_y_points)
    : Mesh(num_x_points, num_y_points, -1.0f, 1.0f, -1.0f, 1.0f)
{
}

Mesh::Mesh(const unsigned int num_x_points, const unsigned int num_y_points,
           const float min_x, const float max_x, const float min_y, const float max_y)
    : NUM_X_POINTS(num_x_points), NUM_Y_POINTS(num_y_points), MIN_X(min_x), MAX_X(max_x), MIN_Y(min_y), MAX_Y(max_y), color(color)
{
    gen_mesh();
}

Mesh::Mesh(const std::vector<Point> &points)
    : NUM_X_POINTS(DEFAULT_POINTS), NUM_Y_POINTS(DEFAULT_POINTS), MIN_X(1.0f), MAX_X(-1.0f), MIN_Y(1.0f), MAX_Y(-1.0f)
{
    unsigned int i;
    for (i = 0; i < points.size(); i++)
    {
        float x = points[i].x;
        float y = points[i].y;

        if (x > MAX_X)
        {
            MAX_X = x;
        }
        else if (x < MIN_X)
        {
            MIN_X = x;
        }

        if (y > MAX_Y)
        {
            MAX_Y = y;
        }
        else if (y < MIN_Y)
        {
            MIN_Y = y;
        }
    }
    // Set the color from the last point (or choose your preferred logic)
    color = points[i - 1].color;
    gen_mesh();
}

Mesh::~Mesh()
{
}

unsigned int Mesh::size() const
{
    return mesh_points.size();
}

const MeshPoint *Mesh::data() const
{
    return mesh_points.data();
}

void Mesh::to_weirstrass()
{
    for (unsigned int i = 0; i < mesh_points.size(); i++)
    {
        mesh_points[i].to_weirstrass();
    }
}

void Mesh::rotateXHyperbolic(float theta)
{
    for (unsigned int i = 0; i < mesh_points.size(); i++)
    {
        mesh_points[i].rotateXHyperbolic(theta);
    }
}
void Mesh::rotateYHyperbolic(float theta)
{
    for (unsigned int i = 0; i < mesh_points.size(); i++)
    {
        mesh_points[i].rotateYHyperbolic(theta);
    }
}

void Mesh::gen_mesh()
{
    mesh_points.reserve(NUM_X_POINTS * NUM_Y_POINTS);

    // Calculate increments outside the loops for clarity
    const float INCR_X = (MAX_X - MIN_X) / NUM_X_POINTS;
    const float INCR_Y = (MAX_Y - MIN_Y) / NUM_Y_POINTS;

    float x = MIN_X;
    float y = MIN_Y;

    // Loop over Y points
    for (unsigned int i = 0; i < NUM_Y_POINTS; i++)
    {
        // Ensure the last row exactly matches MAX_Y
        if (i == (NUM_Y_POINTS - 1))
        {
            y = MAX_Y;
        }
        else
        {
            y = MIN_Y + i * INCR_Y;
        }

        // Loop over X points
        for (unsigned int j = 0; j < NUM_X_POINTS; j++)
        {
            // Ensure the last column exactly matches MAX_X
            if (j == (NUM_X_POINTS - 1))
            {
                x = MAX_X;
            }
            else
            {
                x = MIN_X + j * INCR_X;
            }

            // Compute texture coordinates normalized between 0 and 1.
            float u = (x - MIN_X) / (MAX_X - MIN_X);
            float v = (y - MIN_Y) / (MAX_Y - MIN_Y);

            // Create a MeshPoint using the new constructor signature with UV coordinates.
            MeshPoint p = MeshPoint(x, y, 0.0f, color, i * NUM_Y_POINTS + j, glm::vec2(u, v));

            // Set up neighboring pointers (note: be cautious when referencing elements not yet emplaced)
            // Here, we use indices based on our current understanding of the mesh layout.
            // You might need additional logic if these pointers are accessed immediately.
            if (i == 0)
            {
                p.up = &(mesh_points[(i + 1) * NUM_Y_POINTS + j]);
                p.down = nullptr;
            }
            else if (i == (NUM_Y_POINTS - 1))
            {
                p.up = nullptr;
                p.down = &(mesh_points[(i - 1) * NUM_Y_POINTS + j]);
            }
            else
            {
                p.up = &(mesh_points[(i + 1) * NUM_Y_POINTS + j]);
                p.down = &(mesh_points[(i - 1) * NUM_Y_POINTS + j]);
            }

            if (j == 0)
            {
                p.right = &(mesh_points[i * NUM_Y_POINTS + j + 1]);
                p.left = nullptr;
            }
            else if (j == (NUM_X_POINTS - 1))
            {
                p.right = nullptr;
                p.left = &(mesh_points[i * NUM_Y_POINTS + j - 1]);
            }
            else
            {
                p.right = &(mesh_points[i * NUM_Y_POINTS + j + 1]);
                p.left = &(mesh_points[i * NUM_Y_POINTS + j - 1]);
            }

            mesh_points.emplace_back(p);
        }
    }
}
