#ifndef RENDERHYP_HPP
#define RENDERHYP_HPP

#include <mutex>
#include <condition_variable>
#include <ShaderProgram.hpp>
#include <thread>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <ne_system/Resource.hpp>

#include "TextureManager.hpp"
#include "Camera.hpp"
#include <glad/glad.h>
#include "ne_engine.hpp"

#include <vector>

// Atlas Object Used to render non euclidean math
// TODO ADD ANIMATIONS
struct Character
{
	PQTile tile;
	AtlasedTexture texture;
};
struct AtlasPQtile
{
	PQTile tile;
	AtlasedTexture texture;
	float zIndex;
};
class RendererHyp
{
public:
	RendererHyp() = default;
	~RendererHyp();

	void Init();
	void Render(entt::registry &registry, Resource<Camera> camera) const;
	void Bind();
	void Clear(); // NOT USED
	void renderTiles(entt::registry &registry) const;
	void renderCharacter(entt::registry &registry) const;
	static constexpr int N_INDICES = 6;

private:
	unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO, VE0, UV_VBO2;
	std::unique_ptr<ShaderProgram> _shader_program;
};

#endif // RENDERHYP_HPP