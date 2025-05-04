#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera {
	glm::vec3 position;
	glm::vec4 up;
	glm::mat4 proj_mat;

	Camera() {
		position = glm::vec3 (0.0, 0.0, 2.0);
		up = glm::vec4 (0.0, 1.0, 0.0, 1.0);

		float fov = glm::radians(45.0f);
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		proj_mat = glm::perspective(fov, (800.0f / 600.0f), nearPlane, farPlane);
	}

	Camera(glm::vec3 position, glm::vec4 up, glm::mat4 proj_mat)
		: position(position), up(up), proj_mat(proj_mat) {}

	[[nodiscard]] glm::mat4 GetViewProjMat() const {
		return proj_mat * lookAt(position, glm::vec3(position.x, position.y, 0.0), xyz(up));
	};
};

#endif //CAMERA_HPP
