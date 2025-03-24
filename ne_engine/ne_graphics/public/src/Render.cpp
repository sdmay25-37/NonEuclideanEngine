#include "Render.hpp"

#include <TextureManager.hpp>
#include <glad/glad.h>

Render::~Render() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UV_VBO);
	glDeleteBuffers(1, &MODEL_MAT_VBO);
	glDeleteBuffers(1, &EBO);
}

void Render::init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &UV_VBO);
	glGenBuffers(1, &MODEL_MAT_VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VERTEX BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex texture coords attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// buffer vertex data
	std::vector<Vertex> vertices = {
		Vertex {  0.5,  0.5, 0.0, 1.0, 1.0 },
		Vertex {  0.5, -0.5, 0.0, 1.0, 0.0 },
		Vertex { -0.5, -0.5, 0.0, 0.0, 0.0 },
		Vertex { -0.5,  0.5, 0.0, 0.0, 1.0 }
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// UV BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);

	// sprite texture coords attribute
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	// MODEL MATRIX BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);

	// model matrix attribute
	for(int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(3 + i);
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(3 + i, 1);
	}

	// INDEX BUFFER //
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// buffer index data
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

// Todo: Not sure how I feel about this method
// I don't like having to copy UV data every frame when it likely doesn't change
void Render::render(entt::registry& registry) const {
	auto const view = registry.view<AtlasSprite>();

	std::unordered_map<unsigned int, std::pair<std::vector<glm::mat4>, std::vector<glm::vec4>>> atlas_data;

	// Group sprite texture data by atlas
	for(auto [entity, sprite] : view.each()) {
		auto& [model_mats, uv_ranges] = atlas_data[sprite.texture.atlas_id];
		model_mats.push_back(sprite.model_mat);
		uv_ranges.emplace_back(sprite.texture.uv_min.x, sprite.texture.uv_min.y, sprite.texture.uv_max.x, sprite.texture.uv_max.y);
	}

	// Render one atlas at a time
	for(const auto& [atlas_id, val] : atlas_data) {
		const auto& [model_mats, uv_ranges] = val;

		// Todo: If we are feeling really crazy we could double buffer this data to reduce latency
		// Buffer model matrix data
		glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_mats.size(), model_mats.data(), GL_DYNAMIC_DRAW);

		// Buffer UV data
		glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

		// Bind atlas texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, atlas_id);

		// Render sprites
		glDrawElementsInstanced(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, nullptr, model_mats.size());
	}
}

void Render::bind() {
	glBindVertexArray(VAO);
}