#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera {
	glm::vec3 position;
	glm::vec4 up;
	glm::mat4 proj_mat;

	Camera(glm::vec3 position, glm::vec4 up, glm::mat4 proj_mat)
		: position(position), up(up), proj_mat(proj_mat) {}

	[[nodiscard]] glm::mat4 GetViewProjMat() const {
		return proj_mat * lookAt(position, glm::vec3(position.x, position.y, 0.0), xyz(up));
	};
};

#endif //CAMERA_HPP
