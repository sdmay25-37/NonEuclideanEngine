//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
  
struct Vertex {
    float x, y, z;
    float r, g, b;
};

class Triangle {
public:
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