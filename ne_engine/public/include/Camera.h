//
//  Author: Tasman Grinnell
//  Date: 11/26/24
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera();
    Camera(GLFWwindow* window);

    void moveCameraUp();
    void moveCameraDown();
    void moveCameraLeft();
    void moveCameraRight();

    glm::vec3 getCameraPos();
    glm::vec4 getCameraUp();

private:
    GLFWwindow* window;
    glm::vec3 _camera_pos;
    glm::vec4 _camera_up;
    float _camera_speed;
};

#endif // CAMERA_H