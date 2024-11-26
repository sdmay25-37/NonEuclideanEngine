//
//  Author: Tasman Grinnell
//  Date: 11/26/24
//

Camera::Camera() {
    _camera_pos = (0.0, 0.0, 1.0);
    _camera_speed = 0.05f;
    _camera_up = (0.0, 1.0, 0.0, 1.0);
}

Camera::Camera(GLFWwindow* window) {
    _camera_pos = (0.0, 0.0, 1.0);
    _camera_speed = 0.05f;
}

Camera::moveCameraUp() {
    _camera_pos.y += _camera_speed;
}

Camera::moveCameraDown() {
    _camera_pos.y -= _camera_speed;
}

Camera::moveCameraLeft() {
    _camera_pos.z += _camera_speed;
}

Camera::moveCameraRight() {
    _camera_pos.z -= _camera_speed;
}

Camera::getCameraPos() {
    return _camera_pos;
}

Camera::getCameraUp() {
    return _camera_up;
}