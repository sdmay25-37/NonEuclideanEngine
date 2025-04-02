#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <cstring>

#include "Point.hpp"

struct MeshPoint : public Point
{
    MeshPoint() = default;
    MeshPoint(float x, float y, float z, unsigned int index) : Point(x, y, z, PointType::POINCARE)
    {
        this->index = index;
    }

    unsigned int index;
    MeshPoint* right;
    MeshPoint* up;
    MeshPoint* left;
    MeshPoint* down;
};

class Mesh
{
    public:
    Mesh() = default;
    Mesh(const unsigned int num_x_points, const unsigned int num_y_points);
    Mesh(const unsigned int num_x_points, const unsigned int num_y_points,
               const float min_x, const float max_x, const float min_y, const float max_y);

    Mesh(const std::vector<Point>& points);

    ~Mesh();

    unsigned int size() const;
    const MeshPoint* data() const;

    void to_weirstrass();

    const MeshPoint& operator[](unsigned int index) const
    {
        return mesh_points[index];
    }

    Mesh& operator=(const Mesh& mesh)
    {
        this->NUM_X_POINTS = mesh.NUM_X_POINTS;
        this->NUM_Y_POINTS = mesh.NUM_Y_POINTS;
        this->MIN_X        = mesh.MIN_X;
        this->MAX_X        = mesh.MAX_X;
        this->MIN_Y        = mesh.MIN_Y;
        this->MAX_Y        = mesh.MAX_Y;

        this->gen_mesh();

        return *this;
    }

    private:
    void gen_mesh();

    unsigned int NUM_X_POINTS;
    unsigned int NUM_Y_POINTS;
    float MIN_X;
    float MAX_X;
    float MIN_Y;
    float MAX_Y;

    std::vector<MeshPoint> mesh_points;

};

#endif