#ifndef RENDER_HPP
#define RENDER_HPP

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

#include <vector>

struct Vertex
{
	float x, y, z;
	float u, v;
};

struct Sprite
{
	glm::mat4 model_mat;
	glm::vec2 uv_min, uv_max;
};

struct AtlasSprite
{
	glm::mat4 model_mat;
	AtlasedTexture texture;
};

// Represents a mesh that can be rendered with instancing
struct AtlasMesh
{
	std::vector<glm::vec4> positions;  // vertex positions (x, y, z, w) or homogeneous
	std::vector<glm::vec4> colors;	   // per-vertex color
	std::vector<glm::vec2> uvs;		   // UV coordinates per vertex
	std::vector<unsigned int> indices; // index buffer
	unsigned int texture_id;		   // OpenGL texture handle

	// Adding default value for texture_id
	AtlasMesh(
		std::vector<glm::vec4> positions_,
		std::vector<glm::vec4> colors_,
		std::vector<glm::vec2> uvs_,
		std::vector<unsigned int> indices_,
		unsigned int texture_id_ // Default to 0 if not provided
		)
		: positions(std::move(positions_)),
		  colors(std::move(colors_)),
		  uvs(std::move(uvs_)),
		  indices(std::move(indices_)),
		  texture_id(texture_id_)
	{
	}
};

struct AtlasTexture
{
	unsigned int atlas_id;
	uint32_t tile_index;
	uint32_t tiles_per_row; // e.g., 16 for a 16x16 grid

	glm::vec2 uv(uint32_t i) const
	{
		// Calculate absolute tile index for this mesh vertex
		uint32_t tile = tile_index;

		// Convert index to row/column
		uint32_t col = tile % tiles_per_row;
		uint32_t row = tile / tiles_per_row;

		// Calculate tile UV bounds
		float tile_size = 1.0f / float(tiles_per_row);
		glm::vec2 uv_min = glm::vec2(col * tile_size, row * tile_size);
		glm::vec2 uv_max = uv_min + glm::vec2(tile_size);

		// Compute per-vertex UV within tile using i
		// Here we assume mesh uses a triangle fan or triangle list that matches square tiling
		// Replace this with actual per-vertex UVs if available
		static const glm::vec2 default_uvs[] = {
			{1, 1}, {1, 0}, {0, 0}, {0, 1}, // quad order
			{0.5, 0.5},						// center (optional)
		};

		return uv_min + default_uvs[i % 4] * tile_size;
	}
};

class Renderer
{
public:
	Renderer() = default;
	~Renderer();

	void Init();
	void Render(entt::registry &registry, Resource<Camera> camera) const;
	void Bind();

	static constexpr int N_INDICES = 6;

private:
	unsigned int VBO, UV_VBO, MODEL_MAT_VBO, VAO, EBO, VE0;
	std::unique_ptr<ShaderProgram> _shader_program;
	GLuint mesh_VAO, mesh_VBO, mesh_EBO;
};

#endif // RENDER_HPP
