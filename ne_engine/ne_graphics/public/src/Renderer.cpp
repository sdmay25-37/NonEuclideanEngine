#include "Renderer.hpp"

#include <iostream>
#include <TextureManager.hpp>
#include <glad/glad.h>
#include "ne_engine.hpp"
#include <nlohmann/json.hpp>

// Used to render each NonEuclidean tile in Renderer::render()
// Needed because things are meshes are unique per tile and need to save each data
struct MeshAtlasKeyHash
{
	std::size_t operator()(const std::pair<const void *, unsigned int> &key) const
	{
		std::size_t h1 = std::hash<const void *>{}(key.first);
		std::size_t h2 = std::hash<unsigned int>{}(key.second);
		return h1 ^ (h2 << 1);
	}
};
// Used to render each NonEuclidean tile in Renderer::render()
// Needed because things are meshes are unique per tile and need to save each data
// This isn't needed but good practices suggest it
struct MeshAtlasKeyEqual
{
	bool operator()(const std::pair<const void *, unsigned int> &lhs,
					const std::pair<const void *, unsigned int> &rhs) const
	{
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}
};
Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VE0);
	glDeleteBuffers(1, &UV_VBO);
}
void Renderer::Init()
{
	// Generate all buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VE0);
	glGenBuffers(1, &UV_VBO);

	// --- Instanced Rendering Setup ---
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);

	// THESE CAUSE WARNINGS BUT I DON'T KNOW HOW TO MAKE IT WORK Without it
	// Setting up the buffer for the PQ shaders
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, fraguv));
	glEnableVertexAttribArray(2);

	// sprite texture coords attribute
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1); // THIS IS WHAT CAUSED MY PAIN FOR LIKE 8 HOURS

	// Load shaders
	auto shader_result = ShaderProgram::create(
		"../ne_engine/ne_math/shaders/pq_test.vert",
		"../ne_engine/ne_math/shaders/pq_color.frag");
	if (shader_result.is_error())
	{
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	_shader_program = std::make_unique<ShaderProgram>(shader_result.ok());
	_shader_program->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Todo: Not sure how I feel about this method
// Update: Same Bro this ucks my yum
// I don't like having to copy UV data every frame when it likely doesn't change
void Renderer::Render(entt::registry &registry, Resource<Camera> camera) const
{

	HypRotate r_uniform_matrix = HypRotate(true);

	_shader_program->setUniform1i("texture_atlas", 0);

	_shader_program->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

	auto pqTile_view = registry.view<AtlasPQtile>();

	// Todo There should be a better way to do this
	// But each mesh is unique
	using MeshAtlasKey = std::pair<const void *, unsigned int>;
	std::unordered_map<
		std::pair<const void *, unsigned int>,
		std::vector<const AtlasPQtile *>,
		MeshAtlasKeyHash,
		MeshAtlasKeyEqual>
		batched_tiles;

	for (auto [entity, currentTile] : pqTile_view.each())
	{
		auto mesh_ptr = static_cast<const void *>(currentTile.tile.mesh_data()); // use mesh_data pointer as a key
		auto atlas_id = currentTile.texture.atlas_id;

		batched_tiles[{mesh_ptr, atlas_id}].push_back(&currentTile);
	}

	for (auto &[key, tiles] : batched_tiles)
	{
		const auto &[mesh_key, atlas_id] = key;
		const auto &sample_tile = tiles[0]->tile;
		// Upload mesh data (shared for all instances)
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MeshPoint) * sample_tile.mesh_size(), sample_tile.mesh_data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sample_tile.indices_size(), sample_tile.indices_data(), GL_STATIC_DRAW);

		// Collect per-instance UVs
		std::vector<glm::vec4> uv_ranges;

		for (auto *tile : tiles)
		{
			// tile->tile.to_weirstrass(); // already converted earlier? Do once, not per frame
			const auto &tex = tile->texture;
			uv_ranges.emplace_back(tex.uv_min.x, tex.uv_min.y, tex.uv_max.x, tex.uv_max.y);
		}

		// Upload instance UVs
		glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_id);

		// Draw instances
		glDrawElementsInstanced(GL_TRIANGLES, sample_tile.indices_size(), GL_UNSIGNED_INT, 0, uv_ranges.size());
	}
}

void Renderer::Bind()
{
	glBindVertexArray(VAO);
	_shader_program->bind();
}

// NOT USED
void Renderer::Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}