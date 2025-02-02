//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#include "Triangle.h"

#include <vector>
#include <iostream>

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

Triangle::Triangle(std::vector<struct Vertex> listVert, float sensIn) {
    vertices = listVert;
    sensitivity = sensIn;
}

void Triangle::moveRight() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[0] += sensitivity;
        if (vertices[i].pos[0] >= 1.0f) {
            std::cout << "out of bounds" << std::endl;
            for (int j = i; j >= 0; j--) {
                std::cout << "Revert " << i;
                vertices[i].pos[0] -= sensitivity;
            }
            std::cout <<std::endl;
            break;
        }
    }
}

void Triangle::moveLeft() {
    for (int i = 0; i < 3; i++) {
        vertices[i].pos[0] -= sensitivity;
        if (vertices[i].pos[0] <= -1.0f) {
            for (int j = i; j >= 0; j--) {
                vertices[i].pos[0] += sensitivity;
            }
            break;
        }
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
    std::cout << "return verts: " << vertices.at(0).pos[0] << ", " << vertices.at(0).pos[0] << std::endl;
    return vertices;
}