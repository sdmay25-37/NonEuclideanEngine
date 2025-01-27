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

    Triangle();
    Triangle(std::vector<struct Vertex> vertices);
    Triangle(std::vector<struct Vertex> vertices, float sensitivity);

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    std::vector<struct Vertex> getVerts();
private:
    std::vector<struct Vertex> vertices;
    float sensitivity;
};

#endif // TRIANGLE_H