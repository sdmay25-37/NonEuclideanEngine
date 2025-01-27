//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#include "Triangle.h"

#include <vector>

Triangle::Triangle() {
    vertices = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    };
    sensitivity = 0.1;
}

Triangle::Triangle(std::vector<struct Vertex> listVert) {
    vertices = listVert;
    sensitivity = 0.1;
}

Triangle::Triangle(std::vector<struct Vertex> listVert, float sensitivity) {
    vertices = listVert;
    sensitivity = sensitivity;
}

void Triangle::moveRight() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[0] += sensitivity;
    }
}

void Triangle::moveLeft() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[0] -= sensitivity;
    }
}

void Triangle::moveUp() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[1] += sensitivity;
    }
}

void Triangle::moveDown() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[1] -= sensitivity;
    }
}

std::vector<Triangle::Vertex> Triangle::getVerts() {
    return vertices;
}