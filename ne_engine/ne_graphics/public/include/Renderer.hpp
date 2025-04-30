#ifndef RENDER_HPP
#define RENDER_HPP

#include <ShaderProgram.hpp>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <ne_system/Resource.hpp>

#include "Camera.hpp"
#include "TextureManager.hpp"


struct Vertex {
	float x, y, z;
	float u, v;
};

struct SpriteData {
	unsigned int z_index;
	glm::mat4 model_mat;
	glm::vec4 uvs;

	SpriteData(unsigned int z_index, glm::mat4 model_mat, glm::vec4 uvs)
		: z_index(z_index), model_mat(model_mat), uvs(uvs) {}
};

struct AtlasSprite {
	unsigned int z_index;
	glm::mat4 model_mat;
	AtlasedTexture texture;
};

class Renderer {
public:
	Renderer() = default;
	~Renderer();

	void Init();
	void Render(entt::registry& registry, Resource<Camera> camera) const;
	void Bind();

	static constexpr int N_INDICES = 6;

private:
	unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO;
	std::unique_ptr<ShaderProgram> _shader_program;
};

#endif //RENDER_HPP