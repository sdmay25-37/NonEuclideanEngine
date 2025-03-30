#ifndef RENDER_HPP
#define RENDER_HPP

#include <mutex>
#include <condition_variable>
#include <thread>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "TextureManager.hpp"


struct Vertex {
	float x, y, z;
	float u, v;
};

struct Sprite {
	glm::mat4 model_mat;
	glm::vec2 uv_min, uv_max;
};

struct AtlasSprite {
	glm::mat4 model_mat;
	AtlasedTexture texture;
};

class Renderer {
public:
	Renderer() = default;
	~Renderer();

	void init();
	void render(entt::registry &registry) const;
	void bind();

	// Stages //
	void prepare();
	void queue();
	void batch_sort();
	void render();
	void cleanup();


	static constexpr int N_INDICES = 6;

private:

	unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO;

};

#endif //RENDER_HPP
