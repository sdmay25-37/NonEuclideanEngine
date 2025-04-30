#include "Renderer.hpp"

#include <iostream>
#include <TextureManager.hpp>
#include <glad/glad.h>

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UV_VBO);
	glDeleteBuffers(1, &MODEL_MAT_VBO);
	glDeleteBuffers(1, &EBO);
}

void Renderer::Init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	// build and compile our shader program
	auto shader_result = ShaderProgram::create(
			"../shaders/sprite.vert",
			"../shaders/sprite.frag"
	);

	if(shader_result.is_error()) {
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	_shader_program = std::make_unique<ShaderProgram>(shader_result.ok());
	_shader_program->bind();
	_shader_program->setUniform1i("texture_atlas", 0);
}

// Todo: Not sure how I feel about this method
// I don't like having to copy UV data every frame when it likely doesn't change
void Renderer::Render(entt::registry& registry, Resource<Camera> camera) const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	_shader_program->setUniformMat4("proj_view_mat", camera->GetViewProjMat());

	auto const view = registry.view<AtlasSprite>();

	std::unordered_map<unsigned int, std::vector<SpriteData>> atlas_data;

	// Group sprite texture data by atlas
	for(auto [entity, sprite] : view.each()) {
		auto uvs = glm::vec4(sprite.texture.uv_min.x, sprite.texture.uv_min.y, sprite.texture.uv_max.x, sprite.texture.uv_max.y);
		atlas_data[sprite.texture.atlas_id].emplace_back(sprite.z_index, sprite.model_mat, uvs);
	}

	// Sort sprites by z-index (for each atlas)
	for(auto& [_, list] : atlas_data) {
		std::sort(list.begin(), list.end(), [](const SpriteData& a, const SpriteData& b) {
			return a.z_index < b.z_index;
		});
	}

	// Render one atlas at a time
	for(const auto& [atlas_id, list] : atlas_data) {
		std::vector<glm::mat4> model_mats;
		std::vector<glm::vec4> uv_ranges;

		// Todo: this 'extra' copy to new vectors can be avoided by unpacking SpriteData and sorting both lists at once
		for (const auto& sprite_data : list) {
			model_mats.push_back(sprite_data.model_mat);
			uv_ranges.push_back(sprite_data.uvs);
		}

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

void Renderer::Bind() {
	glBindVertexArray(VAO);
	_shader_program->bind();
}