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

    static void moveCameraUp();
    static void moveCameraDown();
    static void moveCameraLeft();
    static void moveCameraRight();

    static glm::vec3 getCameraPos();
    static glm::vec4 getCameraUp();

private:
    GLFWwindow* window;
    glm::vec3 _camera_pos;
    glm::vec4 _camera_up;
    float _camera_speed;
};

#endif // CAMERA_H