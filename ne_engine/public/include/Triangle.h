//
//  Author: Tasman Grinnell
//  Date: 12/22/24
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
  
struct Vertex {
    float x, y, z;
    float r, g, b;
};

class Triangle {
public :
    Triangle();
    Triangle(Vertex[3] startVert);
    Triangle(Vertex[3] startVert, float sensitivity);

    void moveRight():
    void moveLeft():
    void moveUp():
    void moveDown():
private:
    Vertex[3] vertices;
    float sensitivity;
};

#endif // TRIANGLE_H