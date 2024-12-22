//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#include "Triangle.h"

#include <vector>

struct Vertex {
    float x, y, z;
    float r, g, b;
};

Triangle::Triangle() {
    this.vertices = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    };
    sensitivity = 0.1;
}

Triangle::Triangle(std::vector<struct Vertex> vertices) {
    this.vertices = vertices;
    sensitivity = 0.1;
}

Triangle::Triangle(std::vector<struct Vertex> vertices, float sensitivity) {
    this.vertices = vertices;
    sensitivity = sensitivity;
}

void Triangle::moveRight() {
    for (int i = 0; i < 3; i++) {
        vert.x += sensitivity;
    }
}

void Triangle::moveLeft() {
    for (int i = 0; i < 3; i++) {
        vert.x -= sensitivity;
    }
}

void Triangle::moveUp() {
    for (int i = 0; i < 3; i++) {
        vert.y += sensitivity;
    }
}

void Triangle::moveDown() {
    for (int i = 0; i < 3; i++) {
        vert.y -= sensitivity;
    }
}

std::vector<struct Vertex> Triangle::getVerts() {
    return vertices;
}