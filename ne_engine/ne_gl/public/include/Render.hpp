#ifndef RENDER_HPP
#define RENDER_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

struct Vertex {
	float x, y, z;
	float u, v;
};

struct Sprite {
	glm::mat4 model_mat;
	glm::vec2 uv_min, uv_max;
};

class Render {
public:
	Render();
	~Render();

	void render(entt::registry &registry);
	void bind();

	static constexpr int N_INDICES = 6;

private:
	unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO;

};

#endif //RENDER_HPP
