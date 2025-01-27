//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>

class Triangle {
public:
    struct Vertex {
        float pos[3];
        float color[3];
    };

    std::vector<struct Vertex> vertices;
    float sensitivity;

    Triangle();
    Triangle(std::vector<struct Vertex> vertices);
    Triangle(std::vector<struct Vertex> vertices, float sensitivity);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    std::vector<Triangle::Vertex> getVerts();
};

#endif // TRIANGLE_H