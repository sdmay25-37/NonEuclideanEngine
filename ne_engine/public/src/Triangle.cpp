

#include "Triangle.h"

Triangle::Triangle() {
    vertices = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    };
    sensitivity = 0.1;
}

Triangle::Triangle(Vertex[3] startVert) {
    vertices = startVert;
    sensititvity = 0.1;
}

Triangle::Triangle(Vertex[3] startVert, float sensitivity) {
    vertices = startVert;
    sensititvity = sensitivity;
}

void Triangle::moveRight() {
    for (Vertex vert : vertices) {
        vert.x += sensitivity;
    }
}

void Triangle::moveLeft() {
    for (Vertex vert : vertices) {
        vert.x -= sensitivity;
    }
}

void Triangle::moveUp() {
    for (Vertex vert : vertices) {
        vert.y += sensitivity;
    }
}

void Triangle::moveDown() {
    for (Vertex vert : vertices) {
        vert.y -= sensitivity;
    }
}