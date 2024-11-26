//
//  Author: Tasman Grinnell
//  Date: 11/26/24
//

#include "Camera.h"

Camera::Camera() {
    _camera_pos = glm::vec3 (0.0, 0.0, 1.0);
    _camera_speed = 0.05f;
    _camera_up = glm::vec4 (0.0, 1.0, 0.0, 1.0);
}

Camera::Camera(GLFWwindow* window) {
    _camera_pos = glm::vec3 (0.0, 0.0, 1.0);
    _camera_speed = 0.05f;
}

void Camera::moveCameraUp() {
    _camera_pos.y += _camera_speed;
}

void Camera::moveCameraDown() {
    _camera_pos.y -= _camera_speed;
}

void Camera::moveCameraLeft() {
    _camera_pos.z += _camera_speed;
}

void Camera::moveCameraRight() {
    _camera_pos.z -= _camera_speed;
}

glm::vec3 Camera::getCameraPos() {
    return _camera_pos;
}

glm::vec4 Camera::getCameraUp() {
    return _camera_up;
}