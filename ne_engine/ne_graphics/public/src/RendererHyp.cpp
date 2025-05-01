#include "RendererHyp.hpp"

#include <iostream>
#include <TextureManager.hpp>
#include <glad/glad.h>
#include "ne_engine.hpp"
#include <nlohmann/json.hpp>

// Used to render each NonEuclidean tile in Renderer::render()
// Needed because things are meshes are unique per tile and need to save each data
struct MeshAtlasKeyHash {
	std::size_t operator()(const std::pair<const void *, unsigned int> &key) const {
		std::size_t h1 = std::hash<const void *>{}(key.first);
		std::size_t h2 = std::hash<unsigned int>{}(key.second);
		return h1 ^ (h2 << 1);
	}
};

// Used to render each NonEuclidean tile in Renderer::render()
// Needed because things are meshes are unique per tile and need to save each data
// This isn't needed but good practices suggest it

RendererHyp::~RendererHyp() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VE0);
	glDeleteBuffers(1, &UV_VBO);
	glDeleteBuffers(1, &UV_VBO2);
}

void RendererHyp::Init() {
	// Generate all buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VE0);
	glGenBuffers(1, &UV_VBO);
	glGenBuffers(1, &UV_VBO2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glBlendFunc(GL_SRC_ALPHA, GL_ONE); // or glBlendFunc(GL_ONE, GL_ONE);
	// glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	// glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// --- Instanced Rendering Setup ---
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);

	// THESE CAUSE WARNINGS BUT I DON'T KNOW HOW TO MAKE IT WORK Without it
	// Setting up the buffer for the PQ shaders
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Point), (void *) offsetof(Point, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Point), (void *) offsetof(Point, uv));
	glEnableVertexAttribArray(1);

	// sprite texture coords attribute
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1); // THIS IS WHAT CAUSED MY PAIN FOR LIKE 8 HOURS

	// BIND TO NEW BUFFER TODO
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	// "../ne_engine/shaders/pq_test.vert",
	// 		"../ne_engine/shaders/pq_color.frag"
	// "../ne_engine/ne_math/shaders/pq_test.vert",
	// 	"../ne_engine/ne_math/shaders/pq_color.frag");
	// Load shaders
	auto shader_result = ShaderProgram::create(
		"../ne_engine/shaders/pq_tile.vert",
		"../ne_engine/shaders/pq_tile.frag");
	if(shader_result.is_error()) {
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	_shader_program = std::make_unique<ShaderProgram>(shader_result.ok());
	_shader_program->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Transparent clear color
	glClear(GL_COLOR_BUFFER_BIT);
}

// Todo: Not sure how I feel about this method
// Update: Same Bro this ucks my yum
// I don't like having to copy UV data every frame when it likely doesn't change
void RendererHyp::Render(entt::registry &registry, Resource<Camera> camera) const {
	// Clear the screen (both color and depth buffer)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the rotation matrix
	HypRotate r_uniform_matrix = HypRotate(true);

	// Set uniforms for shader
	_shader_program->setUniform1i("texture_atlas", 0);
	_shader_program->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

	// Create a view of all AtlasPQtile entities in the registry
	auto pqTile_view = registry.view<AtlasPQtile>();

	// Setup batching logic for tiles
	using MeshAtlasKey = std::pair<const void *, unsigned int>;
	std::unordered_map<MeshAtlasKey, std::vector<AtlasPQtile *>, MeshAtlasKeyHash> batched_tiles;

	// Batch tiles by mesh and atlas ID
	int tile_counter = 0;
	for(auto [entity, currentTile]: pqTile_view.each()) {
		auto mesh_ptr = static_cast<const void *>(currentTile.tile.data());
		auto atlas_id = currentTile.texture.atlas_id;

		// Add to the batched tiles map
		batched_tiles[{mesh_ptr, atlas_id}].push_back(&currentTile);
	}

	// Draw each batch of tiles
	int batch_counter = 0;
	for(auto &[key, tiles]: batched_tiles) {
		const auto &[mesh_key, atlas_id] = key;
		auto &sample_tile = tiles[0]->tile;

		// Bind buffers and load data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * sample_tile.data_size(), sample_tile.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * sample_tile.indices_size(),
		             sample_tile.indices_data(), GL_STATIC_DRAW);

		std::vector<glm::vec4> uv_ranges;
		std::vector<float> zindexVector;

		// Collect UV ranges and z-index values for the tiles
		for(auto *tile: tiles) {
			const auto &tex = tile->texture;
			uv_ranges.emplace_back(tex.uv_min.x, tex.uv_min.y, tex.uv_max.x, tex.uv_max.y);
			zindexVector.emplace_back(tile->zIndex);
		}

		glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, UV_VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * zindexVector.size(), zindexVector.data(), GL_STATIC_DRAW);

		// Activate texture unit and bind the texture atlas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_id);

		// Check if the z-index indicates a transparent tile (-1 is transparent)
		bool isTransparent = tiles[0]->zIndex == -1;

		// Draw the batch with instanced rendering
		glDrawElementsInstanced(GL_TRIANGLES, sample_tile.indices_size(), GL_UNSIGNED_INT, 0, tiles.size());
	}
}

void RendererHyp::Bind() {
	glBindVertexArray(VAO);
	_shader_program->bind();
}

// NOT USED
void RendererHyp::Clear() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Transparent clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
