//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

/*
    TODO: REMOVE WHEN NECESSARY

    This is an example object class that I used for testing the Input stuff (JSON, rebinding, etc)
*/

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

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
    Triangle(std::vector<struct Vertex> vertices, float sensIn);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    std::vector<Triangle::Vertex> getVerts();
};

#endif // TRIANGLE_H